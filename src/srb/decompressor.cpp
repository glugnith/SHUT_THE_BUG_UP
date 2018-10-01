#include "srb/decompressor.hpp"

namespace srb
{
    void print_line(std::vector<char> &line)
    {
        std::string str(line.begin(), line.end());
        std::cout << "" << str ;
    }

    void check_readability_of_file(const std::string & file_ext)
    {
        std::set<std::string> readable_extensions = {BZ2,GZ,ZLIB,NO_EXT};

        if (!readable_extensions.count(file_ext)) {
            throw std::invalid_argument{"Input file should not be compressed"};
        }
    }

    void create_uncompressed_stream(std::istream & input,
                                    const std::string & sourceName,
                                    boost::iostreams::filtering_istream & uncompressed_input)
    {
        std::vector<char> line;
        get_magic_num(input, line);

        std::string file_ext = get_compression(sourceName, line);
        check_readability_of_file(file_ext);

        if(file_ext == BZ2) {
            uncompressed_input.push(boost::iostreams::bzip2_decompressor());
        } else if(file_ext == GZ) {
            uncompressed_input.push(boost::iostreams::gzip_decompressor());
        } else if(file_ext == ZLIB) {
            uncompressed_input.push(boost::iostreams::zlib_decompressor());
        }

        uncompressed_input.push(input);
    }

    void get_magic_num(std::istream & stream, std::vector<char> & container)
    {
        char c;
        int i = 0;
        container.clear();

        while (stream && stream.get(c)) {
            container.push_back(c);
            i++;
            if (c == '\n' || i > 4) break;
        }

        for (int j = container.size()-1; j >= 0; --j) {
            stream.putback(container[j]);
            if (stream.fail() || !stream) {
                std::cout << "Failed to unget the characters "
                          << "get_magic_num not working as expected. \n";
            }
        }
    }

    std::string get_compression(std::string const & source,
                                const std::vector<char> &line)
    {

        std::vector<std::pair<std::vector<char>, std::string>> types = {
            { { 66, 90, 104 }, BZ2 },
            { { 31, -117 }, GZ },
            { { -3, 55, 122, 88, 90 }, XZ },
            { { 31, -99 }, Z },
            { { 80, 75, 3, 4 }, ZIP },
            { { 120, -100 }, ZLIB }
        };
        
		/*
		* If the first line of the VCF file has less than 5 characters, 
		* std::equal will cause a segmentation fault.
		*/
		if(line.size()<5)
			return NO_EXT;
        for (auto & type : types) {
            if (std::equal(type.first.begin(), type.first.end(), line.begin())) {
                std::cout << "detected " << type.second
                    << " compression" << std::endl;
                return type.second;
            }
        }
    }

    std::vector<char> & readline(std::istream & stream, std::vector<char> & container)
    {
        char c;
        container.clear();
        while (stream && stream.get(c)) {
            container.push_back(c);
            if (c == '\n') break;
        }
        return container;
    }
}
