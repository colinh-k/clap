#include <iostream>

#include "../include/clap.hpp"
#include "qtest.hpp"

QTestCase(ArgumentParser, TestExtraArgumentAfterFlag) {
    clap::ArgumentParser parser;
    parser.addArg({"--filename"}, "a filename");
    char const *argv[] = {"testProgram", "--filename", "file.txt", "extra_arg"};
    QTEST_EXPECT_THROWS(parser.parse(4, argv), clap::ParseException);
}

QTestCase(ArgumentParser, TestMultipleFlagsOneGiven) {
    clap::ArgumentParser parser;
    parser.addArg({"--destination"}, "a destination");
    parser.addArg({"--source"}, "a source");
    parser.addArg({"--filename"}, "a filename");
    char const *argv[] = {"testProgram", "--filename", "file.txt"};
    clap::ArgumentMap map = parser.parse(3, argv);
    QTEST_EXPECT(map["filename"][0] == "file.txt");
    QTEST_EXPECT(!map.hasValue("destination"));
    QTEST_EXPECT(!map.hasValue("--destination"));
    QTEST_EXPECT(!map.hasValue("source"));
    QTEST_EXPECT(!map.hasValue("--source"));
}

QTestCase(ArgumentParser, TestMultipleFlagsNoneGiven) {
    clap::ArgumentParser parser;
    parser.addArg({"--destination"}, "a destination");
    parser.addArg({"--source"}, "a source");
    parser.addArg({"--filename"}, "a filename");
    char const *argv[] = {"testProgram"};
    clap::ArgumentMap map = parser.parse(1, argv);
    QTEST_EXPECT(!map.hasValue("filename"));
    QTEST_EXPECT(!map.hasValue("--filename"));
    QTEST_EXPECT(!map.hasValue("destination"));
    QTEST_EXPECT(!map.hasValue("--destination"));
    QTEST_EXPECT(!map.hasValue("source"));
    QTEST_EXPECT(!map.hasValue("--source"));
}

QTestCase(ArgumentParser, TestMultipleFlagsAllGiven) {
    clap::ArgumentParser parser;
    parser.addArg({"--destination"}, "a destination");
    parser.addArg({"--source"}, "a source");
    parser.addArg({"--filename"}, "a filename");
    char const *argv[] = {"testProgram", "--source", "src.txt",
                          "--destination", "dest.txt",
                          "--filename", "file.txt"};
    clap::ArgumentMap map = parser.parse(7, argv);
    QTEST_EXPECT(map["filename"][0] == "file.txt");
    QTEST_EXPECT(map["destination"][0] == "dest.txt");
    QTEST_EXPECT(map["source"][0] == "src.txt");
}

QTestCase(ArgumentParser, TestMultipleFlagsJustNameNoArgGiven) {
    clap::ArgumentParser parser;
    parser.addArg({"--destination"}, "a destination");
    parser.addArg({"--source"}, "a source");
    parser.addArg({"--filename"}, "a filename");
    char const *argv[] = {"testProgram", "--source"};
    QTEST_EXPECT_THROWS(parser.parse(2, argv), clap::ParseException);
}

QTestCase(ArgumentParser, TestMultiplePositionalAllGiven) {
    clap::ArgumentParser parser;
    parser.addArg({"destination"}, "a destination");
    parser.addArg({"source"}, "a source");
    parser.addArg({"filename"}, "a filename");
    char const *argv[] = {"testProgram", "dest.txt", "src.txt", "file.txt"};
    clap::ArgumentMap map = parser.parse(4, argv);
    QTEST_EXPECT(map["filename"][0] == "file.txt");
    QTEST_EXPECT(map["destination"][0] == "dest.txt");
    QTEST_EXPECT(map["source"][0] == "src.txt");
}

QTestCase(ArgumentParser, TestMultiplePositionalSomeNotGiven) {
    clap::ArgumentParser parser;
    parser.addArg({"destination"}, "a destination");
    parser.addArg({"source"}, "a source");
    parser.addArg({"filename"}, "a filename");
    char const *argv[] = {"testProgram", "dest.txt", "src.txt"};
    QTEST_EXPECT_THROWS(parser.parse(3, argv), clap::ParseException);
}

QTestCase(ArgumentParser, TestMultiplePositionalExtraGiven) {
    clap::ArgumentParser parser;
    parser.addArg({"destination"}, "a destination");
    parser.addArg({"source"}, "a source");
    parser.addArg({"filename"}, "a filename");
    char const *argv[] = {"testProgram", "dest.txt", "src.txt", "file.txt", "extra_arg"};
    QTEST_EXPECT_THROWS(parser.parse(5, argv), clap::ParseException);
}

QTestCase(ArgumentParser, TestMultiplePositionalNoneGiven) {
    clap::ArgumentParser parser;
    parser.addArg({"destination"}, "a destination");
    parser.addArg({"source"}, "a source");
    parser.addArg({"filename"}, "a filename");
    char const *argv[] = {"testProgram"};
    QTEST_EXPECT_THROWS(parser.parse(1, argv), clap::ParseException);
}

QTestCase(ArgumentParser, TestLongNameOptionalAndPositional) {
    clap::ArgumentParser parser;
    parser.addArg({"destination"}, "a destination");
    parser.addArg({"source"}, "a source");
    parser.addArg({"--filename"}, "a filename");
    parser.addArg({"--verbosity"}, "verbosity level");
    char const *argv[] = {"testProgram", "dest.txt", "src.txt", "--filename", "file.txt", "--verbosity", "high"};
    clap::ArgumentMap map = parser.parse(7, argv);
    QTEST_EXPECT(map["filename"][0] == "file.txt");
    QTEST_EXPECT(map["destination"][0] == "dest.txt");
    QTEST_EXPECT(map["source"][0] == "src.txt");
    QTEST_EXPECT(map["verbosity"][0] == "high");
}

QTestCase(ArgumentParser, TestLongAndShortNameOptionalAndPositional) {
    clap::ArgumentParser parser;
    parser.addArg({"destination"}, "a destination");
    parser.addArg({"source"}, "a source");
    parser.addArg({"--filename", "-f"}, "a filename");
    parser.addArg({"--verbosity", "-v"}, "verbosity level");
    char const *argv[] = {"testProgram", "dest.txt", "src.txt", "-v", "high", "-f", "file.txt"};
    clap::ArgumentMap map = parser.parse(7, argv);
    QTEST_EXPECT(map["filename"][0] == "file.txt");
    QTEST_EXPECT(map["destination"][0] == "dest.txt");
    QTEST_EXPECT(map["source"][0] == "src.txt");
    QTEST_EXPECT(map["verbosity"][0] == "high");
}

QTestCase(ArgumentParser, TestLongAndShortNameOptionalAndPositionalRandomOrder) {
    clap::ArgumentParser parser;
    parser.addArg({"destination"}, "a destination");
    parser.addArg({"source"}, "a source");
    parser.addArg({"--filename", "-f"}, "a filename");
    parser.addArg({"--verbosity", "-v"}, "verbosity level");
    char const *argv[] = {"testProgram", "-v", "high", "dest.txt", "-f", "file.txt", "src.txt"};
    clap::ArgumentMap map = parser.parse(7, argv);
    QTEST_EXPECT(map["filename"][0] == "file.txt");
    QTEST_EXPECT(map["destination"][0] == "dest.txt");
    QTEST_EXPECT(map["source"][0] == "src.txt");
    QTEST_EXPECT(map["verbosity"][0] == "high");
}

QTestCase(ArgumentParser, TestLongAndShortNameOptionalAndPositionalNoFlagsGiven) {
    clap::ArgumentParser parser;
    parser.addArg({"destination"}, "a destination");
    parser.addArg({"source"}, "a source");
    parser.addArg({"--filename", "-f"}, "a filename");
    parser.addArg({"--verbosity", "-v"}, "verbosity level");
    char const *argv[] = {"testProgram", "dest.txt", "src.txt"};
    clap::ArgumentMap map = parser.parse(3, argv);
    QTEST_EXPECT(map["destination"][0] == "dest.txt");
    QTEST_EXPECT(map["source"][0] == "src.txt");
    QTEST_EXPECT(!map.hasValue("verbosity"));
    QTEST_EXPECT(!map.hasValue("filename"));
}

QTestCase(ArgumentParser, TestLongAndShortNameOptionalAndPositionalSomeFlagsGiven) {
    clap::ArgumentParser parser;
    parser.addArg({"destination"}, "a destination");
    parser.addArg({"source"}, "a source");
    parser.addArg({"--filename", "-f"}, "a filename");
    parser.addArg({"--verbosity", "-v"}, "verbosity level");
    char const *argv[] = {"testProgram", "dest.txt", "-f", "file.txt", "src.txt"};
    clap::ArgumentMap map = parser.parse(5, argv);
    QTEST_EXPECT(map["destination"][0] == "dest.txt");
    QTEST_EXPECT(map["source"][0] == "src.txt");
    QTEST_EXPECT(map["filename"][0] == "file.txt");
    QTEST_EXPECT(!map.hasValue("verbosity"));
}

QTestCase(ArgumentParser, TestOptionalAndPositionalExtraArgInFlag) {
    clap::ArgumentParser parser;
    parser.addArg({"destinations"}, "destinations");
    parser.addArg({"source"}, "a source");
    parser.addArg({"--filename", "-f"}, "a filename");
    parser.addArg({"--verbosity", "-v"}, "verbosity level");
    char const *argv[] = {"testProgram", "dest1.txt", "-f", "file.txt", "dest2.txt", "src.txt"};
    QTEST_EXPECT_THROWS(parser.parse(6, argv), clap::ParseException);
}

QTestCase(ArgumentParser, TestMultipleArgsPerOption) {
    clap::ArgumentParser parser;
    parser.addArg({"--destinations"}, "destinations", 2);
    parser.addArg({"--sources"}, "sources", 3);
    parser.addArg({"--filename"}, "a filename");
    char const *argv[] = {"testProgram", "--destinations", "dest1.txt", "dest2.txt",
                          "--sources", "src1.txt", "src2.txt", "src3.txt",
                          "--filename", "file.txt"};
    clap::ArgumentMap map = parser.parse(10, argv);
    QTEST_EXPECT(map["destinations"][0] == "dest1.txt");
    QTEST_EXPECT(map["destinations"][1] == "dest2.txt");
    QTEST_EXPECT(map["sources"][0] == "src1.txt");
    QTEST_EXPECT(map["sources"][1] == "src2.txt");
    QTEST_EXPECT(map["sources"][2] == "src3.txt");
    QTEST_EXPECT(map["filename"][0] == "file.txt");
}

QTestCase(ArgumentParser, TestHelpBasic) {
    clap::ArgumentParser parser;
    parser.addArg({"--destinations"}, "destinations", 2);
    parser.addArg({"--sources"}, "sources", 3);
    parser.addArg({"--filename"}, "a filename");
    char const *argv[] = {"testProgram", "-h"};
    QTEST_EXPECT_THROWS(parser.parse(2, argv), clap::HelpException);
}

QTestCase(ArgumentParser, TestHelpWithOtherFlags) {
    clap::ArgumentParser parser;
    parser.addArg({"--destinations"}, "destinations", 2);
    parser.addArg({"--sources"}, "sources", 3);
    parser.addArg({"--filename"}, "a filename");
    char const *argv[] = {"testProgram", "--destinations",
                          "dest1", "dest2", "-h",
                          "--sources", "src1", "src2", "src3"};
    QTEST_EXPECT_THROWS(parser.parse(9, argv), clap::HelpException);

    clap::ArgumentParser parser2;
    parser2.addArg({"--destinations"}, "destinations", 2);
    parser2.addArg({"--sources"}, "sources", 3);
    parser2.addArg({"--filename"}, "a filename");
    char const *argv2[] = {"testProgram", "--destinations",
                          "dest1", "-h", "dest2",
                          "--sources", "src1", "src2", "src3"};
    QTEST_EXPECT_THROWS(parser2.parse(9, argv2), clap::HelpException);
}

QTestCase(ArgumentMap, TestSingleFlagOnlyLongNameLookup) {
    clap::ArgumentParser parser;
    parser.addArg({"--filename"}, "a filename");
    char const *argv[] = {"testProgram", "--filename", "file.txt"};
    clap::ArgumentMap map = parser.parse(3, argv);
    QTEST_EXPECT(map["filename"][0] == "file.txt");
    QTEST_EXPECT(map["--filename"][0] == "file.txt");
    QTEST_EXPECT_THROWS(map["-filename"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["f"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["-f"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["--f"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["invalid_name"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["-invalid_name"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["--invalid_name"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["-i"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["--i"], clap::InvalidNameException);
}

QTestCase(ArgumentMap, TestSingleFlagWithShortNameLookup) {
    clap::ArgumentParser parser;
    parser.addArg({"--filename", "-f"}, "a filename");
    char const *argv[] = {"testProgram", "--filename", "file.txt"};
    clap::ArgumentMap map = parser.parse(3, argv);
    QTEST_EXPECT(map["filename"][0] == "file.txt");
    QTEST_EXPECT(map["--filename"][0] == "file.txt");
    QTEST_EXPECT(map["-f"][0] == "file.txt");
    QTEST_EXPECT(map["f"][0] == "file.txt");

    QTEST_EXPECT_THROWS(map["-filename"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["--f"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["invalid_name"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["-invalid_name"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["--invalid_name"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["-i"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["--i"], clap::InvalidNameException);
}

QTestCase(ArgumentMap, TestSinglePositionalArgLookup) {
    clap::ArgumentParser parser;
    parser.addArg({"filename"}, "a filename");
    char const *argv[] = {"testProgram", "file.txt"};
    clap::ArgumentMap map = parser.parse(2, argv);
    QTEST_EXPECT(map["filename"][0] == "file.txt");

    QTEST_EXPECT_THROWS(map["-filename"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["--filename"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["--f"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["-f"], clap::InvalidNameException);
    QTEST_EXPECT_THROWS(map["f"], clap::InvalidNameException);
}

int main(int argc, char const *argv[]) {
    // NOTE: all of these test arguments that expect exactly one arg
    // ArgumentParser, parse: tests for parsing arguments
    // 16
    QTestRegister(ArgumentParser, TestExtraArgumentAfterFlag);
    QTestRegister(ArgumentParser, TestMultipleFlagsOneGiven);
    QTestRegister(ArgumentParser, TestMultipleFlagsNoneGiven);
    QTestRegister(ArgumentParser, TestMultipleFlagsAllGiven);
    QTestRegister(ArgumentParser, TestMultipleFlagsJustNameNoArgGiven);
    QTestRegister(ArgumentParser, TestMultiplePositionalAllGiven);
    QTestRegister(ArgumentParser, TestMultiplePositionalSomeNotGiven);
    QTestRegister(ArgumentParser, TestMultiplePositionalExtraGiven);
    QTestRegister(ArgumentParser, TestMultiplePositionalNoneGiven);
    QTestRegister(ArgumentParser, TestLongNameOptionalAndPositional);
    QTestRegister(ArgumentParser, TestLongAndShortNameOptionalAndPositional);
    QTestRegister(ArgumentParser, TestLongAndShortNameOptionalAndPositionalRandomOrder);
    QTestRegister(ArgumentParser, TestLongAndShortNameOptionalAndPositionalNoFlagsGiven);
    QTestRegister(ArgumentParser, TestLongAndShortNameOptionalAndPositionalSomeFlagsGiven);
    QTestRegister(ArgumentParser, TestMultipleArgsPerOption);
    QTestRegister(ArgumentParser, TestOptionalAndPositionalExtraArgInFlag);
    QTestRegister(ArgumentParser, TestHelpBasic);
    QTestRegister(ArgumentParser, TestHelpWithOtherFlags);
    // ArgumentParser, addArg: tests for adding arguments
    // ArgumentMap: tests for looking up arguments after parsing
    // 3
    QTestRegister(ArgumentMap, TestSingleFlagOnlyLongNameLookup);
    QTestRegister(ArgumentMap, TestSingleFlagWithShortNameLookup);
    QTestRegister(ArgumentMap, TestSinglePositionalArgLookup);

    // TODO: test ArgumentMap::hasValue() by itself

    QTestRunAll();
    return 0;
}