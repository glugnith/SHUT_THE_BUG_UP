#ifndef SRB_DECOMPRESSOR_HPP
#define SRB_DECOMPRESSOR_HPP

#include <iostream>
#include <set>
#include <string>
#include <vector>


#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/zlib.hpp>

#include "string_constants.hpp"

namespace srb
{

    struct Hell {
        int a;
        int b;
        
        void print();
    };

    void check_readability_of_file(const std::string & file_ext);

    std::vector<char> & readline(std::istream & stream, std::vector<char> & container);

    void get_magic_num(std::istream & stream, std::vector<char> & container);

    std::string get_compression(std::string const & source,
                                const std::vector<char> &line);

    void create_uncompressed_stream(std::istream & input,
                                    const std::string & sourceName,
                                    boost::iostreams::filtering_istream & uncompressed_input);


    void print_line(std::vector<char> &line);
}

#endif
