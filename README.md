# Outline
- add expected arguments to an Argspp object in order, or give optional positional priority
  - attempt to parse the arg with the given data type (could be object type or only integral type ?)
- print help message if parsing fails
- support optional args, with default value
- support 'exactly one of many' args
- support positional args
- support flag args
- support retrieving arg values via indexing: `<Argspp object>[<arg name string>]`
  - key should be string ?
  - or should retrieving be done via >> operator ? No, bc we want to get by argument name
<!-- - perform parsing on any given character stream (see ctor) -->
- support taking a list of valid arguments and verifying the actual arguments are from this list; e.g., one of `{yes, no, maybe}` for an argument
  - remember to check that the given options are the same type as the argument

# TODO
- ensure that each named argument has a unique name, and throw an error otherwise
- instead of throwing exceptions, maybe just show usage ?
- hint for parameterized parsing type: require the type to support `>>` operator
  - also, make a template class to store type information, like `clap::Type<int>()`
- allow metavaribale option ?
  - like `F F F`, instead of `filename filename filename`
- add a `hasValue/exists(std::string name)` function to query whether an argument was parsed with a value
- to avoid using `shared_ptr`, maintain a vector of `ArgInfo`. then, map names to indexes in this vector

# Development Version Notes
## Version 1.0
- parse string arguments
- all args are required
- no flags; only positional arguments supported
- every argument value access returns a vector of strings
- able to specify the number of arguments expected for each positional argument, or default to 1
- prints usage message if unable to parse the arguments

## Version 2.0
- **arguments must have a long name**
- arguments may optionally have a short name
  - `{"filename", "f"} -> {--filename, -f}`
- arguments may be specified as positional
- arguments may be specified as required
- a positional argument must be required
- features from python's argparse:
  - an argument is optional if a long name with a '--' prefix is provided
    - e.g. `[--verbosity VERBOSITY]`
  - an argument is positional if a long name without a '--' prefix is provided
- in summary, an argument is considered optional if:
  - its long name is prefixed with '--' and its short name is empty, or
  - its long name is prefixed with '--' and its short name is a single character prefixed with '-'
- required/positional:
  - long name without '--' implies positional argument, and the short name must be empty
- only possible combinations (first two are optinoal, last is required):
  - `--filename, -f`, `--filename, ''`, `filename, ''`

## Version 3.0
- [x] add `-h` help option
- [x] each argument has a description
  - descriptions are printed with the `-h` option (not when an error occurs)
- [x] return a new object from `parse()` with the argument map (and usage information), so the parser object can go out of scope without losing the argument map for lookup. also allows the map to not depend on the parsing object
- [x] add a description to the parser in the parser's ctor
- [x] store arguments in the map with their dash prefixes

## Version 4.0
- [ ] add default value that is used if no actual arguments are given
- [ ] if a default is given, the argument is not required in the actual arg list
  - [ ] but it can still have `-/--` prefixes in its name(s) or no prefixes (thus being positional) since a default value is always given when it is omitted in the actual arg list
- [ ] add an option list
  - [ ] the actual argument must be a value from this list / set
  - [ ] print usage if the arg is not from this set
  - [ ] throw error / check if options in the list are not of the same expected type as the argument ?
    - [ ] is this necessary ? or can we just print usage when the actual arg is not in that list ? or will this be caught when the actual arg is from that list of the wrong type, when parsing is attempted as the wrong type ?

## Version 5.0
- [ ] add a type system to allow adding arguments with an expected type
  - [ ] **require the type to support `operator>>`**
  - [ ] for collection types (like `std::vector`), allow an optional `nargs` parameter to specify how many arguments are expected
  - [ ] for all other types, a single argument is assumed