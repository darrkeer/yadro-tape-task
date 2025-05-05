#include <iostream>

#include "tape-algo.h"

#include <string>
#include <fstream>
#include <filesystem>
#include <algorithm>

constexpr std::size_t FACTOR = 100;

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Error: incorrect arguments! Usage: tape-sort in.txt out.txt!";
        return 1;
    }
    std::string in = argv[1];
    std::ifstream in_stream(in);
    if (!in_stream.is_open()) {
        std::cerr << "Error: unable to open file '" + in + "'!";
        return 1;
    }
    std::string binary_filename = tape_config::convert_to_binary_file(in_stream);
    std::size_t n = std::filesystem::file_size(binary_filename) / sizeof(int);
    std::size_t m = std::max(1UL, n / FACTOR);

    tape t(n, m, binary_filename);
    tape_algo::sort(t);

    std::string out = argv[2];
    std::ofstream out_stream(out);
    if (!out_stream.is_open()) {
        std::cerr << "Error: unable to write in file '" + out + "'!";
        return 1;
    }
    out_stream << t;
}