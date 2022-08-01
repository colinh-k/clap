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

    parser.addArg({"operation"}, "specify the type of operation to perform {encrypt or decrypt}", clap::Type<std::string>({"encrypt", "decrypt"}));
    parser.addArg({"input", "-i"}, "the input file", clap::Type<std::string>());
    parser.addArg({"output", "-o"}, "the output filename", clap::Type<std::string>());
    parser.addArg({"key", "-k"}, "the key file", clap::Type<std::string>());
    parser.addArg({"double", "-d"}, "a double", clap::Type<double>({1.5, 2.5, 3.5}));
    parser.addArg({"--range", "-r"}, "range for operation {first-byte last-byte}", clap::Type<std::vector<int>>({0, 10}), 2);

    std::size_t argc = 10;
    // char const *argv[] = {"program_name", "--help"};
    char const *argv[] = {
        "./utility", "encrypt",
        "-i", "input.txt",
        "-o", "output.txt",
        "-k", "key.txt",
        "-d", "1.6"
        // "--range", "5", "6"
    };

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

    std::string op = map.get<std::string>("operation");
    std::cout << "operation: " << op << '\n';

    std::string input = map.get<std::string>("input");
    std::cout << "input: " << input << '\n';

    std::string output = map.get<std::string>("output");
    std::cout << "output: " << output << '\n';
    
    std::string key = map.get<std::string>("key");
    std::cout << "key: " << key << '\n';

    double dub = map.get<double>("double");
    std::cout << "dub: " << dub << '\n';
    
    std::vector<int> range = map.get<std::vector<int>>("range");
    std::cout << "range: " << range[0] << ' ' << range[1] << '\n';

    return EXIT_SUCCESS;
}

/*
int main() {
    // an example program to demonstrate the library
    clap::ArgumentParser parser;

    parser.addArg({"--source", "-s"}, "a source file", clap::Type<std::vector<std::string>>(), 5);
    parser.addArg({"--destination", "-d"}, "a destination file", clap::Type<std::string>("default"));
    parser.addArg({"verbosity"}, "a required verbosity option", clap::Type<std::string>({"high", "low", "off"}));
    parser.addArg({"fname"}, "a filename", clap::Type<std::string>());
    parser.addArg({"--range"}, "specify range bounds", clap::Type<int>());

    std::size_t argc = 11;
    // char const *argv[] = {"program_name", "--help"};
    char const *argv[] = {"program_name", "file.txt",
                          "--source", "src.txt", "src2.txt", "src3.txt", "src4.txt", "src5.txt",
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
    std::string dest = map.get<std::string>("d");
    std::string verb = map.get<std::string>("verbosity");

    std::cout << "destination: " << dest << '\n';
    std::cout << "verbosity: " << verb << '\n';
    // std::cout << "source: " << source << '\n';
    for (auto& s : source) {
        std::cout << s << " ";
    }
    std::cout << '\n';
    std::cout << "fname: " << fname << '\n';
    std::cout << "range1: " << range1 << '\n';

    return 0;
}
*/
