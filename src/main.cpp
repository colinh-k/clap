#include <iostream>

#include "../include/clap.hpp"

int main(int argc, char const *argv[]) {
    // clap::ArgumentParser parser("a program to do something special");
    clap::ArgumentParser parser;

    parser.addArg({"--boolean", "-b"}, "a boolean value");
    parser.addArg({"--fname", "-f"}, "filename", 5);
    parser.addArg({"range"}, "specify range bounds", 3);

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