#include <iostream>

#include "clap.hpp"

int main(int argc, char const *argv[]) {
    clap::CommandLineArgumentParser clap;

    clap.addArg({"--boolean", "-b"});
    clap.addArg({"--fname", "-f"});
    clap.addArg({"range"}, 3);
    clap.parse(argc, argv);

    auto range = clap["range"];
    for (int i = 0; i < range.size(); i++) {
        std::cout << range[i] << "\n";
    }

    for (const auto& x : clap["boolean"]) {
        std::cout << x << '\n';
    }

    for (const auto& x : clap["fname"]) {
        std::cout << x << '\n';
    }

    std::cout << "done\n";
    // std::cout << clap["what"];
    return 0;
}