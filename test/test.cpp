#include <iostream>

#include "../include/clap.hpp"

int main(int argc, char const *argv[]) {
    clap::ArgumentParser parser;

    parser.addArg({"--boolean", "-b"});
    parser.addArg({"--fname", "-f"});
    parser.addArg({"range"}, 3);

    clap::ArgumentMap map = parser.parse(argc, argv);

    for (const auto& x : map["range"]) {
        std::cout << x << '\n';
    }

    for (const auto& x : map["b"]) {
        std::cout << x << '\n';
    }

    for (const auto& x : map["f"]) {
        std::cout << x << '\n';
    }

    return 0;
}

/*
 * std::cout.setstate(std::ios_base::failbit); // off
 * std::cout.clear()  // on
 */