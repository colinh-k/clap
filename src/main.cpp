#include <iostream>
#include <string>

#include "../include/clap.hpp"

#include <vector>

template <typename T>
struct is_cont {
    static const bool value = false;
};

template <typename T>
struct is_cont<std::vector<T>> {
    static const bool value = true;
};

int main() {
    // an example program to demonstrate the library
    clap::ArgumentParser parser;

    parser.addArg({"--source", "-s"}, "a source file", clap::makeValueType<std::vector<std::string>>(), 5);
    parser.addArg({"--destination", "-d"}, "a destination file", clap::makeValueType<std::string>());
    parser.addArg({"fname"}, "a filename", clap::makeValueType<std::string>());
    parser.addArg({"--range"}, "specify range bounds", clap::makeValueType<int>());

    std::size_t argc = 10;
    // char const *argv[] = {"program_name", "--help"};
    char const *argv[] = {"program_name", "file.txt",
                          "--source", "src.txt", "src2.txt", "src3.txt", "src4.txt", "src5.txt",
                        //   "-d", "dest.txt",
                          "--range", "5"};
    clap::ArgumentMap map;
    try {
        map = parser.parse(argc, argv);
    } catch (clap::HelpException& e) {
        // help flag was supplied
        std::cout << parser.help();
        return EXIT_SUCCESS;
    } catch (clap::ClapException& e) {
        // parsing failed
        std::cerr << e.what() << '\n';
        std::cerr << parser.getUsage() << '\n';
        return EXIT_FAILURE;
    }

    std::vector<std::string> source = map.get<std::vector<std::string>>("s");
    std::string fname = map.get<std::string>("fname");
    int range1 = map.get<int>("range");

    // std::cout << "destination: " << dest << '\n';
    // std::cout << "source: " << source << '\n';
    for (auto& s : source) {
        std::cout << s << " ";
    }
    std::cout << '\n';
    std::cout << "fname: " << fname << '\n';
    std::cout << "range1: " << range1 << '\n';

    return 0;
}
