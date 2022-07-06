#pragma once

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <cstdlib>

#include "parse_exception.hpp"

namespace clapp {
    struct ArgumentType {
        ArgumentType(const std::string& name, const int& nargs) :
                _name(name), _nargs(nargs) {}

        // ArgumentType(const ArgumentType& copyme) = default;
        ArgumentType() = default;  // required for std::map to default construct elements
        // ArgumentType& operator=(const ArgumentType& rhs) = default;

        std::string _name;
        // char _shortName;
        int _nargs;
        std::vector<std::string> _values;
    };

    class CommandLineArgumentParser {
        public:
            CommandLineArgumentParser() {}

            void addArg(const std::string& name,
                        const int& nargs=1) {
                if (!nameToArgumentType.count(name)) {
                    // nameToArgumentType[name]{ArgumentType(name, nargs)};
                    nameToArgumentType.emplace(name, ArgumentType(name, nargs));
                    positionalArgs.push_back(&nameToArgumentType[name]);
                }
            }

            std::vector<std::string>& operator[](const std::string& name) {
                return nameToArgumentType[name]._values;
            }

            void parse(int argc, char const **argv) {
                if (!argc) {
                    // TODO: should we accept 0 arguments when attempting to parse ?.
                    // below, we assume at least one argument (the filename)
                    throw ParseException("No arguments availible to parse");
                }
                // TODO: expect the first argument to be the filename, so
                // deal with this later to build the usage string. for now,
                // just ignore it
                std::string fname = argv[0];
                argv = &argv[1];
                argc--;

                int argi = 0;  // index into the actual argument list
                for (int i = 0; i < positionalArgs.size(); i++) {
                    // iterate over the positional args in order
                    ArgumentType& curArg = *(positionalArgs[i]);
                    int nargs = curArg._nargs;
                    for (int j = 0; j < nargs; j++) {
                        // consume the next nargs argumenmts for the current positional argument
                        if (argi >= argc) {
                            // throw an exception here, since we are attempting
                            // to parse more arguments than exist in the actual
                            // argument list
                            // throw ParseException("Attempting to parse more arguments than appear in the actual argument list.");
                            usage(fname);
                        }
                        std::string arg{argv[argi]};
                        curArg._values.push_back(arg);
                        argi++;
                    }
                }

                if (argi != argc) {
                    // throw ParseException("Not enough arguments given.");
                    usage(fname);
                }
            }

        private:
            std::map<std::string, ArgumentType> nameToArgumentType;
            // store pointers to the argument info type in the order added
            std::vector<ArgumentType*> positionalArgs;

            void usage(const std::string& fname) {
                std::cout << "Usage: " << fname << " ";
                for (const ArgumentType* arg : positionalArgs) {
                    for (std::size_t i = 0; i < arg->_nargs; i++) {
                        std::cout << arg->_name << " ";
                    }
                }
                std::cout << std::endl;
                // TODO: output a help description (also add a -h option)
                std::exit(EXIT_SUCCESS);
            }
    };
};