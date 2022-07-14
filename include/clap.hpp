#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <memory>

#include <stdexcept>  // for standard exception classes
#include "parse_exception.hpp"

namespace clap {
    bool isShortFlag(const std::string& name) {
        // short flags have the form '-c', where c can be any character
        // return name.rfind("-", 0);
        return name.size() == 2 && name.at(0) == '-';
    }

    bool isLongFlag(const std::string& name) {
        // long flags have the form '--name', where name is any sequence of
        // at least one character.
        // TODO: this definiton does not include the string '--', which might have a special meaning
        return name.size() >= 3 && name.rfind("--", 0) == 0;
        // return name.size() >= 3 && name.substr(0, 2) == "--";
    }

    bool isFlag(const std::string& name) {
        // is either long or short flag
        return isShortFlag(name) || isLongFlag(name);
    }

    // TODO: should this be a struct or class ?
    class ArgInfo {
        public:
            ArgInfo(const std::string& _longName,
                    const std::string& _shortName,
                    const std::string& _description,
                    const int& _nargs) {
                // TODO: consider moving this logic to a function, and remove the isRequred variable.
                // this may save space at the cost of more run time, since isRequired is used a lot.
                // for now, ive made the decision to compute if each argument is required once and store that
                // NOTE: if we would like to allow the _longName to be a short flag (and the _shortName is
                // empty) to allow the user to only specify a short flag, maybe just replace
                // !isLongFlag(_longName) with !isFlag(_longName) ?
                shortName = _shortName;
                nargs = _nargs;
                hasValue = false;
                longName = _longName;
                description = _description;

                isRequired = !isLongFlag(_longName);
                // the name to represent the argument in the usage message
                const std::string& repeatName = (!isRequired) ? _longName.substr(2) : _longName;

                usage = std::string();
                for (std::size_t i = 0; i < nargs; i++) {
                    usage += repeatName + ' ';
                }
                // remove trailing space
                usage.pop_back();

                if (!isRequired) {
                    // optional argument
                    const std::string& argName = (shortName.empty()) ? longName : shortName;
                    usage = '[' + argName + ((usage.empty()) ? "" : ' ' + usage) + ']';
                }
            }

            // ArgInfo(const ArgInfo& copyme) = default;
            ArgInfo() = default;  // required for std::map to default construct elements
            // ArgInfo& operator=(const ArgInfo& rhs) = default;

            void addValue(const std::string& value) {
                values.push_back(value);
                hasValue = true;
            }

            std::string longName;
            std::string shortName;
            int nargs;
            bool isRequired;  // is a required argument (specified by user)
            bool hasValue;    // whether a value was added to this argument or not (for testing if this argument was given in the actual arg list)
            std::vector<std::string> values;
            // TODO: consider implementing operator<< instead of a usage variable
            std::string usage;
            std::string description;
    };

    struct ArgNames {
        // shortname will be empty if no shortname was given
        ArgNames(const std::string& _longName,
                const std::string& _shortName=std::string()) :
                longName(_longName), shortName(_shortName) {};
        std::string longName, shortName;
    };

    class ArgumentMap {
        public:
            ArgumentMap(std::map<std::string, std::shared_ptr<ArgInfo>>&& _nameToArg, std::string&& _usage) :
                nameToArg(_nameToArg), usage(_usage) {}

            std::vector<std::string>& operator[](const std::string& name) {
                // lookup by either long name or short name, or throw if
                // the name does not exist
                if (nameToArg.count(name)) {
                    return nameToArg[name]->values;
                }
                if (nameToArg.count("-" + name)) {
                    return nameToArg["-" + name]->values;
                }
                if (nameToArg.count("--" + name)) {
                    return nameToArg["--" + name]->values;
                }
                throw std::invalid_argument("argument name '" + name + "' does not exist");
            }

        private:
            std::map<std::string, std::shared_ptr<ArgInfo>> nameToArg;
            std::string usage;
    }; // class ArgumentMap

    class ArgumentParser {
        public:
            ArgumentParser(const std::string& _description=std::string()) {
                description = _description;
                // add the first reserved argument
                std::shared_ptr<ArgInfo> argInfo = std::make_shared<ArgInfo>("--help", "-h", "display this help message", 0);
                nameToArg[argInfo->longName] = argInfo;
                nameToArg[argInfo->shortName] = argInfo;
                optionalArgs.push_back(argInfo);
            }

            void addArg(const ArgNames& names,
                        const std::string& description,
                        const int& nargs=1) {
                // check for valid parameters
                if (names.longName.empty()) {
                    throw std::invalid_argument("a long name is required for every argument");
                } else if (!isLongFlag(names.longName) && !(names.shortName.empty())) {
                    throw std::invalid_argument("a positional argument may not have a short name");
                } else if (!isShortFlag(names.shortName) && !names.shortName.empty()) {
                    throw std::invalid_argument("invalid short name '" + names.shortName + "'. must have the form '-c', where 'c' can be any single character");
                } else if (isShortFlag(names.longName)) {
                    throw std::invalid_argument("long name '" + names.longName + "' may not be a short flag");
                } else if (nameToArg.count(names.longName)) {
                    throw std::invalid_argument("long name '" + names.longName + "' already exists");
                } else if (nameToArg.count(names.shortName)) {
                    throw std::invalid_argument("short name '" + names.shortName + "' already exists");
                }

                // TODO: consider moving the isRequired logic from ArgInfo ctor
                // to here, so we can avoid storing argInfo.long/shortName.
                // remember to also strip the '-/--' prefixes if we move the logic.
                // NOTE: we can pass an isRequired param to the ArgInfo ctor instead
                std::shared_ptr<ArgInfo> argInfo = std::make_shared<ArgInfo>(names.longName, names.shortName, description, nargs);
                // TODO: consider storing the names with their prefixes ('-/--') in
                // the map. this will prevent long names from being used with positional
                // arguments ?
                nameToArg[argInfo->longName] = argInfo;
                nameToArg[argInfo->shortName] = argInfo;
                // store a pointer to the argument info in order
                // TODO: only do this if the name is supposed to be positional,
                // i.e., only when no flag name is specified ?
                if (argInfo->isRequired) {
                    positionalArgs.push_back(argInfo);
                } else {
                    optionalArgs.push_back(argInfo);
                }
            }

            ArgumentMap parse(int argc, char const **argv) {
                if (!argc) {
                    // TODO: should we accept 0 arguments when attempting to parse ?.
                    // below, we assume at least one argument (the filename)
                    // throw ParseException("No arguments availible to parse");
                    // usage("<program name>");
                    // error("no arguments availible to parse.");
                    throw std::invalid_argument("no arguments available to parse");
                }

                // TODO: expect the first argument to be the filename;
                // consider adding the filename in the ctor
                fname = argv[0];
                argv = &argv[1];
                argc--;

                // create the usage string once
                makeUsage();

                std::size_t posi = 0;  // index into positional arg list
                std::size_t argi = 0;  // index into actual argument list
                while (argi < argc) {
                    std::string arg{argv[argi]};

                    if (std::string(argv[argi]) == "-h" || std::string(argv[argi]) == "--help") {
                        // special help case
                        help();
                    }

                    std::shared_ptr<ArgInfo> argInfo = nullptr;

                    // if (isFlag(arg) && nameToArg.count(arg)) {

                    // }

                    if (isFlag(arg)) {
                        if (nameToArg.count(arg)) {
                            argInfo = nameToArg[arg];
                            argi++;  // consume this argument as a flag name
                        } else {
                            // invalid flag
                            error("unrecognized flag '" + arg + "'");
                        }
                    }

                    // we must check if the name[1:] exists in the name map,
                    // since the first character is '-', and the map does not store prefix dashes
                    // std::size_t dashOffset = 0;
                    // if (isShortFlag(arg)) {
                    //     if (nameToArg.count(arg.substr(1))) {
                    //         argInfo = nameToArg[arg.substr(1)];
                    //         argi++;  // consume this argument as a flag name
                    //     } else {
                    //         // invalid flag
                    //         error("unrecognized short flag '" + arg + "'");
                    //     }
                    // } else if (isLongFlag(arg)) {
                    //     if (nameToArg.count(arg.substr(2))) {
                    //         argInfo = nameToArg[arg.substr(2)];
                    //         argi++;  // consume this argument as a flag name
                    //     } else {
                    //         // invalid flag
                    //         error("unrecognized long flag '" + arg + "'");
                    //     }
                    // }
                    // else if (arg == "-") {
                    //     // character '-', which we interpret as an invalid flag
                    //     error("unrecognized flag '-'");
                    // }
                    else {
                        // non flag argument, so intepret it as the next positional argument
                        if (posi < positionalArgs.size()) {
                            argInfo = positionalArgs[posi];
                            // update the current positional argument index
                            posi++;
                            // do not consume the current argument (by updating argi) because
                            // we need to add it to the argument list below since it
                            // is a positional argument
                        } else {
                            // if the index is out of bounds, report an error
                            error("unexpected positional argument '" + arg + "'");
                        }
                    }

                    for (std::size_t j = 0; j < argInfo->nargs; j++) {
                        if (argi >= argc) {
                            // index out of range for actual argument list
                            error("expected " + std::to_string(argInfo->nargs) + " argument(s) for '" + argInfo->longName + "'");
                        }
                        if (std::string(argv[argi]) == "-h" || std::string(argv[argi]) == "--help") {
                            // special help case
                            help();
                        }
                        if (isFlag(argv[argi])) {
                            error("unexpected flag '" + std::string(argv[argi]) + "' when parsing argument '" + argInfo->longName + "'");
                        }
                        // consume the next nargs arguments and store them
                        // in the argInfo struct
                        // increment
                        argInfo->addValue(argv[argi]);
                        argi++;
                    }
                }

                // check if all the required arguments recieved a value
                for (auto& pair : nameToArg) {
                    if (pair.second->isRequired && !(pair.second->hasValue)) {
                        error("'" + pair.second->longName + "' is required");
                    }
                }

                return ArgumentMap(std::move(nameToArg), std::move(usage));
            }

        private:
            // store names without dash prefixes as keys: e.g. f, filename
            std::map<std::string, std::shared_ptr<ArgInfo>> nameToArg;
            // store pointers to the argument info type in the order added
            std::vector<std::shared_ptr<ArgInfo>> positionalArgs;
            // pointers to optional / flag / non-required arguments
            std::vector<std::shared_ptr<ArgInfo>> optionalArgs;  // TODO: could this be a std::set ?
            // usage strings:
            // TODO: maybe we need to collect the boolean optional args into a single
            // string to display together like [-abc], instead of [-a] [-b] [-c] ?
            // std::string optionalNames;
            // std::string requiredNames;
            std::string fname;
            std::string usage;
            // a description of the program to be displayed with help
            std::string description;

            void makeUsage() {
                usage = "Usage: " + fname;
                for (const auto& optionalArg : optionalArgs) {
                    usage += " " + optionalArg->usage;
                }
                for (const auto& positionalArg : positionalArgs) {
                    usage += " " + positionalArg->usage;
                }
                usage += '\n';
                // TODO: output a help description (also add a -h option)
            }

            void help() {
                std::cout << usage << "\n";

                if (!description.empty()) {
                    std::cout << "Description:\n\t" << description << "\n\n";
                }

                std::cout << "Positional arguments:\n";
                for (const auto& positionalArg : positionalArgs) {
                    std::cout << "\t" << positionalArg->longName
                              << "\t" << positionalArg->description << "\n";
                }

                std::cout << "\nOptional arguments:\n";
                for (const auto& optionalArg : optionalArgs) {
                    const std::string& names = (optionalArg->shortName.empty()) ?
                                               optionalArg->longName :
                                               optionalArg->shortName + ", " + optionalArg->longName;
                    std::cout << "\t" << names
                              << "\t" << optionalArg->description << "\n";
                }
                std::exit(EXIT_SUCCESS);
            }

            void error(const std::string& errorStr) {
                // print the error string and exit with failure code
                std::cout << usage;
                std::cerr << "error: " << errorStr << std::endl;
                std::exit(EXIT_FAILURE);
            }
    };  // class ArgumentParser
};  // namespace clap