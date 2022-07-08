#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <cstdlib>

#include <stdexcept>  // for standard exception classes
#include "parse_exception.hpp"

namespace clap {
    struct ArgInfo {
        ArgInfo(const std::string& _longName,
                const std::string& _shortName,
                const int& _nargs) :
                longName(_longName),
                shortName(_shortName),
                nargs(_nargs) {}

        // ArgInfo(const ArgInfo& copyme) = default;
        ArgInfo() = default;  // required for std::map to default construct elements
        // ArgInfo& operator=(const ArgInfo& rhs) = default;

        void addValue(const std::string& value) {
            values.push_back(value);
        }

        std::string longName;
        std::string shortName;
        // char _shortName;
        int nargs;
        std::vector<std::string> values;
    };

    struct ArgNames {
        // shortname will be empty if no shortname was given
        ArgNames(const std::string& _longName,
                const std::string& _shortName=std::string()) :
                longName(_longName), shortName(_shortName) {};
                // _longName("--" + longName), _shortName((shortName.empty()) ? shortName : "-" + shortName) {};
        bool operator<(const ArgNames& other) {
            // required to use as a key in std::map
            return longName < other.longName;
        }
        std::string longName, shortName;
    };

    class CommandLineArgumentParser {
        public:
            CommandLineArgumentParser() {}

            void addArg(const ArgNames& names,
                        const int& nargs=1) {
                if (longNameToType.count(names.longName)) {
                    // long name already in use
                    throw std::invalid_argument("long name '" + names.longName + "' already exists");
                }
                if (shortNameToType.count(names.shortName)) {
                    // short name already in use
                    throw std::invalid_argument("short name '" + names.shortName + "' already exists");
                }

                // long names are required, so every arg has an entry in this map
                longNameToType.emplace(names.longName, ArgInfo(names.longName, names.shortName, nargs));
                // store a pointer to the argument info to avoid copying arg info structs
                shortNameToType.emplace(names.shortName, &longNameToType[names.longName]);
                // store a pointer to the argument info in order
                // TODO: only do this if the name is supposed to be positional,
                // i.e., only when no flag name is specified ?
                positionalArgs.push_back(&longNameToType[names.longName]);
            }

            std::vector<std::string>& operator[](const std::string& name) {
                // lookup by either long name or short name, or throw if
                // the name does not exist
                if (longNameToType.count(name)) {
                    return longNameToType[name].values;
                }
                if (shortNameToType.count(name)) {
                    return shortNameToType[name]->values;
                }
                throw std::invalid_argument("argument name '" + name + "' does not exist");
            }

            void parse(int argc, char const **argv) {
                if (!argc) {
                    // TODO: should we accept 0 arguments when attempting to parse ?.
                    // below, we assume at least one argument (the filename)
                    // throw ParseException("No arguments availible to parse");
                    usage("<program>");
                    error("no arguments availible to parse.");
                }

                // TODO: expect the first argument to be the filename;
                // consider adding the filename in the ctor
                fname = argv[0];
                argv = &argv[1];
                argc--;

                std::size_t posi = 0;  // index into positional arg list
                for (std::size_t i = 0; i < argc; i++) {
                    std::string arg{argv[i]};

                    ArgInfo* argInfo = nullptr;
                    // we must check if the name[1:] exists in the short name map,
                    // since the first character is '-', and the map does not store prefix dashes
                    if (isShortName(arg)) {
                        if (shortNameToType.count(arg.substr(1))) {
                            // valid short name
                            argInfo = shortNameToType[arg.substr(1)];
                            // increment the current argument list index to avoid
                            // adding the flag to the argument's value list
                            i++;
                        } else {
                            // invalid flag
                            usage(fname);
                            error("unrecognized short flag '" + arg + "'");
                        }
                    } else if (isLongName(arg)) {
                        if (longNameToType.count(arg.substr(2))) {
                            // valid long name
                            argInfo = &(longNameToType[arg.substr(2)]);
                            // increment the current argument list index to avoid
                            // adding the flag to the argument's value list
                            i++;
                        } else {
                            // invalid flag
                            usage(fname);
                            error("unrecognized long flag '" + arg + "'");
                        }
                    } else if (arg == "-") {
                        // character '-', which we interpret as an invalid flag
                        usage(fname);
                        error("unrecognized flag '-'");
                    } else {
                        // non flag argument, so intepret it as the next positional argument
                        if (i < positionalArgs.size()) {
                            argInfo = positionalArgs[posi];
                            // update the current positional argument
                            posi++;
                        } else {
                            // if the index is out of bounds, report an error
                            usage(fname);
                            error("unexpected positional argument '" + arg + "'");
                        }
                    }

                    // // parse the current argument as a flag, or as a positional
                    // // argument
                    // ArgInfo argInfo = getArgInfo(arg, posi);

                    for (std::size_t j = 0; j < argInfo.nargs; j++) {
                        // consume the next nargs arguments and store them
                        // in the argInfo struct
                        // increment
                        i++;
                        argInfo.addValue(argv[i]);
                    }

                    // if (isShortName(arg)) {

                    // } else if (isLongName(arg)) {

                    // } else if (arg == "-") {
                    //     // character '-', which we interpret as an invalid flag
                    //     usage(fname);
                    //     error("unrecognized flag '-'");
                    // } else {
                    //     // non flag argument, so intepret it as the next positional argument
                    // }


                    // if (arg.at(0) == '-') {
                    //     // beginning of name handle
                    //     if (arg.size() == 1) {
                            // // character '-', which we interpret as an invalid flag
                            // usage(fname);
                            // error("unrecognized flag '-'");
                    //     } else if (arg.at(1) == '-') {
                    //         // long name
                    //     } else {
                    //         // short name
                    //     }
                    // } else {
                    //     // positional argument
                    // }
                }

                if (posi != positionalArgs.size()) {
                    // TODO: we require every argument to have a value
                }

                // int argi = 0;  // index into the actual argument list
                // for (int i = 0; i < positionalArgs.size(); i++) {
                //     // iterate over the positional args in order
                //     ArgInfo& curArg = *(positionalArgs[i]);
                //     int nargs = curArg._nargs;
                //     for (int j = 0; j < nargs; j++) {
                //         // consume the next nargs argumenmts for the current positional argument
                //         if (argi >= argc) {
                //             // throw an exception here, since we are attempting
                //             // to parse more arguments than exist in the actual
                //             // argument list
                //             // throw ParseException("Attempting to parse more arguments than appear in the actual argument list.");
                //             usage(fname);
                //             error("attempting to parse more arguments than appear in the actual argument list");
                //         }
                //         std::string arg{argv[argi]};
                //         curArg._values.push_back(arg);
                //         argi++;
                //     }
                // }

                // if (argi != argc) {
                //     // not enough arguments given in the actual argument
                //     // list to be parsed with the current rules
                //     // throw ParseException("Not enough arguments given.");
                //     usage(fname);
                //     error("expected more argumnets to parse current argument rules");
                // }
            }

        private:
            std::map<std::string, ArgInfo> longNameToType;
            std::map<std::string, ArgInfo*> shortNameToType;
            // store pointers to the argument info type in the order added
            std::vector<ArgInfo*> positionalArgs;
            std::string fname;

            void usage(const std::string& fname) {
                std::cout << "Usage: " << fname << " ";
                for (const ArgInfo* arg : positionalArgs) {
                    for (std::size_t i = 0; i < arg->nargs; i++) {
                        std::cout << arg->longName << " ";
                    }
                }
                std::cout << std::endl;
                // TODO: output a help description (also add a -h option)
            }

            void error(const std::string& errorStr) {
                // print the error string and exit with failure code
                // TODO: (or should it be success ?)
                std::cerr << "error: " << errorStr << std::endl;
                std::exit(EXIT_FAILURE);
            }

            bool isShortName(const std::string& name) {
                // short names have the form '-c', where c can be any character
                // return name.rfind("-", 0);
                return name.size() == 2 && name.at(0) == '-';
            }

            bool isLongName(const std::string& name) {
                // long names have the form '--name', where name is any sequence of
                // at least one character.
                // TODO: this definiton does not include the string '--', which might have a special meaning
                return name.rfind("--", 0);
                // return name.size() >= 3 && name.substr(0, 2) == "--";
            }

            ArgInfo& getArgInfo(const std::string& name, std::size_t& i) {
                // we must check if the name[1:] exists in the short name map,
                // since the first character is '-', and the map does not store prefix dashes
                if (isShortName(name)) {
                    if (shortNameToType.count(name.substr(1))) {
                        // valid short name
                        return *(shortNameToType[name.substr(1)]);
                    } else {
                        // invalid flag
                        usage(fname);
                        error("unrecognized short flag '" + name + "'");
                    }
                } else if (isLongName(name)) {
                    if (longNameToType.count(name.substr(2))) {
                        // valid long name
                        return longNameToType[name.substr(2)];
                    } else {
                        // invalid flag
                        usage(fname);
                        error("unrecognized long flag '" + name + "'");
                    }
                } else if (name == "-") {
                    // character '-', which we interpret as an invalid flag
                    usage(fname);
                    error("unrecognized flag '-'");
                } else {
                    // non flag argument, so intepret it as the next positional argument
                    if (i < positionalArgs.size()) {
                        ArgInfo* ret = positionalArgs[i];
                        // update the index variable in the caller via reference
                        // to update the current positional argument
                        i++;
                        return *ret;
                    } else {
                        // if the index is out of bounds, report an error
                        usage(fname);
                        error("unexpected positional argument '" + name + "'");
                    }
                }
            }
    };
};