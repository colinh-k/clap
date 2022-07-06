# Requirements
- add expected arguments to an Argspp object in order, or give optional positional priority
  - attempt to parse the arg with the given data type (could be object type or only integral type ?)
- print help message if parsing fails
- support optional args, with default value
- support 'exactly one of many' args
- support positional args
- support flag args
- support retrieving arg values via indexing: '\<Argspp object>[\<arg name string>]'
  - key should be string ?
  - or should retrieving be done via >> operator ? No, bc we want to get by argument name
<!-- - perform parsing on any given character stream (see ctor) -->
- support taking a list of valid arguments and verifying the actual arguments are from this list; e.g., one of {yes, no, maybe} for an argument

# TODO
- ensure that each named argument has a unique name, and throw an error otherwise
- instead of throwing exceptions, maybe just show usage ?
- hint for parameterized parsing type: require the type to support >> operator

# Version Expectations
## Version 1.0
- parse string arguments
- all args are required
- no flags; only positional arguments supported
- every argument value access returns a vector of strings
- able to specify the number of arguments expected for each positional argument, or default to 1
- prints usage message if unable to parse the arguments