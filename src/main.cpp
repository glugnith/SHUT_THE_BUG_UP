#include <srb/decompressor.hpp>


namespace po = boost::program_options;

po::options_description build_command_line_options()
{
    std::string version = "srb_decompressor";
    po::options_description description("Usage: srb_decompressor [OPTIONS] [< input_file]\nAllowed options");

    description.add_options()
        (srb::HELP_OPTION, "Display this help")
        (srb::VERSION_OPTION, "Display version of the decompressor")
        (srb::INPUT_OPTION, po::value<std::string>()->default_value(srb::STDIN), "Path to the input file, or stdin")
        (srb::OUTDIR_OPTION, po::value<std::string>()->default_value(""), "Output directory")
    ;

    return description;
}

int check_command_line_options(po::variables_map const & vm, po::options_description const & desc)
{
    if (vm.count(srb::HELP)) {
        std::cout << desc << std::endl;
        return -1;
    }

    return 0;
}

std::string get_output_path(const std::string &outdir, const std::string &file_path)
{
    if (outdir == "") {
        return file_path;
    }

    boost::filesystem::path file_boost_path{file_path};
    boost::filesystem::path outdir_boost_path{outdir};
    if (!boost::filesystem::exists(outdir_boost_path)) {
        throw std::invalid_argument{"Directory not found: " + outdir_boost_path.string()};
    }
    if (!boost::filesystem::is_directory(outdir_boost_path)) {
        throw std::invalid_argument{"outdir should be a directory, not a file: " + outdir_boost_path.string()};
    }

    outdir_boost_path /= file_boost_path;

    return outdir_boost_path.string();
}

int main (int argc, char** argv) {
    po::options_description desc = build_command_line_options();
    po::variables_map vm;

    po::parsed_options parsed = po::command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    po::store(parsed,vm);
    std::vector<std::string> unused_args = collect_unrecognized(parsed.options, po::include_positional);

    std::cout << "Commandline args are" << std::endl;
    for (auto arg: unused_args) {
        std::cout << arg << std::endl;
    }

    po::notify(vm);
    int check_options = check_command_line_options(vm, desc);
    if (check_options < 0) { return 0; }

    auto path = vm[srb::INPUT].as<std::string>();
    auto outdir = get_output_path(vm[srb::OUTDIR].as<std::string>(), path);

    boost::iostreams::filtering_istream in;
    boost::iostreams::filtering_istream cinn(std::cin);
    srb::create_uncompressed_stream(cinn,"",in);

    std::vector<char> line;
    while (srb::readline(in, line).size() != 0) {
        srb::print_line(line);
    }

    srb::Hell hell = srb::Hell();
    hell.a = 1;

    return 0;
}
