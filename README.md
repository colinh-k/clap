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
  - possible combinations:
    - `--filename, -f`, `--filename, ''`, `filename, ''`