#include <iostream>

#include "../include/clap.hpp"
#include "qtest.hpp"

// turn off output streams
// #define OUT_OFF() std::cout.setstate(std::ios_base::failbit); std::cerr.setstate(std::ios_base::failbit)
// // turn output streams on
// #define OUT_ON()  std::cout.clear(); std::cerr.clear()

clap::ArgumentMap setup(std::size_t argc, char const *argv[]) {
    clap::ArgumentParser parser;
    parser.addArg({"--filename"}, "a filename");
    return parser.parse(argc, argv);
}

QTestCase(ArgumentParser, SingleArgParsing) {
    clap::ArgumentParser parser1;
    parser1.addArg({"--filename"}, "a filename");
    char const *argv1[] = {"testProgram", "--filename", "file.txt", "extra_arg"};
    // QTEST_EXPECT_THROWS(parser1.parse(3, argv1))
}

QTestCase(ArgumentMap, SingleArgLookup) {
    clap::ArgumentParser parser1;
    parser1.addArg({"--filename"}, "a filename");
    char const *argv1[] = {"testProgram", "--filename", "file.txt"};
    clap::ArgumentMap map1 = parser1.parse(3, argv1);
    QTEST_EXPECT(map1["filename"][0] == "file.txt");
    QTEST_EXPECT(map1["--filename"][0] == "file.txt");
    QTEST_EXPECT_THROWS(map1["-filename"], std::invalid_argument);
    QTEST_EXPECT_THROWS(map1["f"], std::invalid_argument);
    QTEST_EXPECT_THROWS(map1["-f"], std::invalid_argument);
    QTEST_EXPECT_THROWS(map1["--f"], std::invalid_argument);
    QTEST_EXPECT_THROWS(map1["invalid_name"], std::invalid_argument);
    QTEST_EXPECT_THROWS(map1["-invalid_name"], std::invalid_argument);
    QTEST_EXPECT_THROWS(map1["--invalid_name"], std::invalid_argument);
    QTEST_EXPECT_THROWS(map1["-i"], std::invalid_argument);
    QTEST_EXPECT_THROWS(map1["--i"], std::invalid_argument);

    clap::ArgumentParser parser2;
    parser2.addArg({"--filename", "-f"}, "a filename");
    char const *argv2[] = {"testProgram", "--filename", "file.txt"};
    clap::ArgumentMap map2 = parser2.parse(3, argv2);
    QTEST_EXPECT(map2["filename"][0] == "file.txt");
    QTEST_EXPECT(map2["--filename"][0] == "file.txt");
    QTEST_EXPECT(map2["-f"][0] == "file.txt");
    QTEST_EXPECT(map2["f"][0] == "file.txt");
    QTEST_EXPECT_THROWS(map2["-filename"], std::invalid_argument);
    // QTEST_EXPECT_THROWS(map2["f"], std::invalid_argument);
    QTEST_EXPECT_THROWS(map2["--f"], std::invalid_argument);
    QTEST_EXPECT_THROWS(map2["invalid_name"], std::invalid_argument);
    QTEST_EXPECT_THROWS(map2["-invalid_name"], std::invalid_argument);
    QTEST_EXPECT_THROWS(map2["--invalid_name"], std::invalid_argument);
    QTEST_EXPECT_THROWS(map2["-i"], std::invalid_argument);
    QTEST_EXPECT_THROWS(map2["--i"], std::invalid_argument);
}

QTestCase(ArgumentMap, LookupException) {
    clap::ArgumentParser parser;
    parser.addArg({"--filename"}, "a filename");
    char const *argv[] = {"testProgram", "--filename", "file.txt"};
    clap::ArgumentMap map = parser.parse(3, argv);
    QTEST_EXPECT_THROWS(map["invalid_name"], std::invalid_argument);
}

int main(int argc, char const *argv[]) {
    // ArgumentParser, parse: tests for parsing arguments
    // ArgumentParser, addArg: tests for adding arguments
    // ArgumentMap: tests for looking up arguments after parsing
    QTestRegister(ArgumentMap, SingleArgLookup);
    QTestRegister(ArgumentMap, LookupException);
    QTestRunAll();
    return 0;
}