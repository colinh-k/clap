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

// template <typename T>
// struct identity { typedef T type; };

// identity<decltype(mySet)>::type::value_type pos;

// template <typename T>
// void fromString(const std::string& str, T& value) {
//     if (is_cont<T>::value) {
//         typename T::value_type tv;
//         std::stringstream stream;
//         stream << str;
//         stream >> tv;
//         char c;
//         if (stream.fail() || stream.get(c)) {
//             // not an integer
//             // return false;
//             throw std::invalid_argument("not correct type");
//         }
//         value.insert(value.iterator(), tv);
//     } else {
//         std::stringstream stream;
//         stream << str;
//         stream >> value;
//         char c;
//         if (stream.fail() || stream.get(c)) {
//             // not an integer
//             // return false;
//             throw std::invalid_argument("not correct type");
//         }
//     }
// }

int main() {
    // an example program to demonstrate the library
    clap::ArgumentParser parser;

    parser.addArg({"--source", "-s"}, "a source file", clap::makeValueType<std::string>());
    parser.addArg({"--destination", "-d"}, "a destination file", clap::makeValueType<std::string>());
    parser.addArg({"fname"}, "a filename", clap::makeValueType<std::string>());
    parser.addArg({"--range"}, "specify range bounds", clap::makeValueType<std::string>());

    std::size_t argc = 6;
    // char const *argv[] = {"program_name", "--help"};
    char const *argv[] = {"program_name", "file.txt",
                          "--source", "src.txt",
                        //   "-d", "dest.txt",
                          "--range", "5-10"};
    clap::ArgumentMap map;
    try {
        map = parser.parse(argc, argv);
    } catch (clap::HelpException& e) {
        // help flag was supplied
        std::cout << parser.help();
        return EXIT_SUCCESS;
    } catch (clap::ParseException& e) {
        // parsing failed
        std::cerr << e.what() << '\n';
        std::cerr << parser.getUsage() << '\n';
        return EXIT_FAILURE;
    }

    std::string source = map["s"].as<std::string>();
    std::string fname = map["fname"].as<std::string>();
    std::string range1 = map["range"].as<std::string>();

    // std::cout << "destination: " << dest << '\n';
    std::cout << "source: " << source << '\n';
    std::cout << "fname: " << fname << '\n';
    std::cout << "range1: " << range1 << '\n';

    return 0;
}
