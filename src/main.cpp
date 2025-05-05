#include <iostream>

#include "tape-algo.h"

#include <string>
#include <fstream>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Error: incorrect arguments! Usage: tape-sort in.bin out.txt!";
    }
    std::string in = argv[1];
    std::string out = argv[2];
    // tape t
}