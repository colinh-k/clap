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
- perform parsing on any given character stream (see ctor)


## details
- # clapp
