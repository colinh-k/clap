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
                    const int& _nargs) {
                    // remove the '-' or '--' prefix if they exist
                    // longName(isLongFlag(_longName) ? _longName.substr(2) : _longName),
                    // shortName(isShortFlag(_longName) ? _shortName.substr(1) : _shortName),
                    // nargs(_nargs),
                    // TODO: consider moving this logic to a function, and remove the isRequred variable.
                    // this may save space at the cost of more run time, since isRequired is used a lot.
                    // for now, ive made the decision to compute if each argument is required once and store that
                    // NOTE: if we would like to allow the _longName to be a short flag (and the _shortName is
                    // empty) to allow the user to only specify a short flag, maybe just replace
                    // !isLongFlag(_longName) with !isFlag(_longName) ?
                    // isRequired(!isLongFlag(_longName) && _shortName.empty()),
                    // hasValue(false) {
                // usage((shortName.empty()) ? "--" + longName : "-" + shortName);
                shortName = (_shortName.empty()) ? _shortName : _shortName.substr(1);
                nargs = _nargs;
                hasValue = false;

                if (isLongFlag(_longName)) {
                    // optional argument
                    longName = _longName.substr(2);
                    isRequired = false;

                    // usage format for optional arg
                    const std::string& argName = (shortName.empty()) ? "--" + longName : "-" + shortName;
                    usage = "[" + argName;
                    // NOTE: we are guaranteed nargs is at least 1
                    // TODO: when allowing boolean arguments, we cannot assume this; will it still format correctly ?
                    for (std::size_t i = 0; i < nargs; i++) {
                        usage += " " + longName;
                    }
                    usage += "]";
                } else {
                    // positional argument
                    longName = _longName;
                    isRequired = true;

                    // TODO: optimize ?
                    // usage format for postitional arg
                    usage = "";
                    for (std::size_t i = 0; i < nargs; i++) {
                        usage += longName + " ";
                    }
                    // remove the trailing space
                    usage.pop_back();
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
            // char _shortName;
            int nargs;
            bool isRequired;  // is a required argument (specified by user)
            bool hasValue;    // whether a value was added to this argument or not (for testing if this argument was given in the actual arg list)
            std::vector<std::string> values;
            // TODO: consider implementing operator<< instead of a usage variable
            std::string usage;
    };

    struct ArgNames {
        // shortname will be empty if no shortname was given
        ArgNames(const std::string& _longName,
                const std::string& _shortName=std::string()) :
                longName(_longName), shortName(_shortName) {};
                // longName("--" + _longName), shortName((_shortName.empty()) ? _shortName : "-" + _shortName) {};
        // bool operator<(const ArgNames& other) {
        //     // required to use as a key in std::map
        //     return longName < other.longName;
        // }
        std::string longName, shortName;
    };

    class CommandLineArgumentParser {
        public:
            CommandLineArgumentParser() {}

            void addArg(const ArgNames& names,
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
                std::shared_ptr<ArgInfo> argInfo = std::make_shared<ArgInfo>(names.longName, names.shortName, nargs);
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
                    // requiredNames += " " + argInfo->longName;
                } else {
                    optionalArgs.push_back(argInfo);
                    // append the short flag name
                    // optionalNames += argInfo->shortName;
                }
            }

            std::vector<std::string>& operator[](const std::string& name) {
                // lookup by either long name or short name, or throw if
                // the name does not exist
                if (nameToArg.count(name)) {
                    return nameToArg[name]->values;
                }
                throw std::invalid_argument("argument name '" + name + "' does not exist");
            }

            void parse(int argc, char const **argv) {
                if (!argc) {
                    // TODO: should we accept 0 arguments when attempting to parse ?.
                    // below, we assume at least one argument (the filename)
                    // throw ParseException("No arguments availible to parse");
                    usage("<program name>");
                    error("no arguments availible to parse.");
                }

                // TODO: expect the first argument to be the filename;
                // consider adding the filename in the ctor
                fname = argv[0];
                argv = &argv[1];
                argc--;

                std::size_t posi = 0;  // index into positional arg list
                std::size_t argi = 0;  // index into actual argument list
                while (argi < argc) {
                    std::string arg{argv[argi]};

                    std::shared_ptr<ArgInfo> argInfo = nullptr;

                    if (isFlag(arg) && nameToArg.count(arg)) {

                    }

                    // we must check if the name[1:] exists in the name map,
                    // since the first character is '-', and the map does not store prefix dashes
                    // std::size_t dashOffset = 0;
                    if (isShortFlag(arg)) {
                        if (nameToArg.count(arg.substr(1))) {
                            argInfo = nameToArg[arg.substr(1)];
                            argi++;  // consume this argument as a flag name
                        } else {
                            // invalid flag
                            usage(fname);
                            error("unrecognized short flag '" + arg + "'");
                        }
                    } else if (isLongFlag(arg)) {
                        if (nameToArg.count(arg.substr(2))) {
                            argInfo = nameToArg[arg.substr(2)];
                            argi++;  // consume this argument as a flag name
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
                        if (posi < positionalArgs.size()) {
                            argInfo = positionalArgs[posi];
                            // update the current positional argument index
                            posi++;
                            // do not consume the current argument (by updating argi) because
                            // we need to add it to the argument list below since it
                            // is a positional argument
                        } else {
                            // if the index is out of bounds, report an error
                            usage(fname);
                            error("unexpected positional argument '" + arg + "'");
                        }
                    }

                    // // parse the current argument as a flag, or as a positional
                    // // argument
                    // ArgInfo argInfo = getArgInfo(arg, posi);

                    for (std::size_t j = 0; j < argInfo->nargs; j++) {
                        if (argi >= argc) {
                            // index out of range for actual argument list
                            usage(fname);
                            error("expected more arguments for '" + argInfo->longName + "'");
                        }
                        if (isFlag(argv[argi])) {
                            usage(fname);
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
                        usage(fname);
                        error("'" + pair.second->longName + "' requires a value");
                    }
                }
                // if (posi != positionalArgs.size()) {
                //     // TODO: we require every argument to have a value
                // }
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

            void usage(const std::string& fname) {
                std::cout << "Usage: " << fname;
                for (const auto& optionalArg : optionalArgs) {
                    std::cout << " " + optionalArg->usage;
                }
                for (const auto& positionalArg : positionalArgs) {
                    std::cout << " " + positionalArg->usage;
                }
                // for (const auto& pair : nameToArg) {
                //     for (std::size_t i = 0; i < pair.second->nargs; i++) {
                //         std::cout << pair.second->longName << " ";
                //     }
                // }
                std::cout << std::endl;
                // TODO: output a help description (also add a -h option)
            }

            void error(const std::string& errorStr) {
                // print the error string and exit with failure code
                // TODO: (or should it be success ?)
                std::cerr << "error: " << errorStr << std::endl;
                std::exit(EXIT_FAILURE);
            }
    };
};  // namespace clap