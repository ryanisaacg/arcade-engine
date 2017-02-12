# Conventions

Coding styles and conventions, mostly so I remember them for myself. They follow in no particular order.

## Type definitions

- If a type never needs to be referred to by name, it should not have a name (anonymous inner structs).
- All types should be used through typedef where possible.
- Types should be in PascalCase.
- Never typedef a number, struct, union, or enum pointer.
- Always typedef function pointers.
- Type names should be nouns

##Functions

- Functions should either be public or private. Public functions should be exposed in headers, private functions should be marked 'static'.
- Function names should be in snake_case, staring with the type prefix they pertain to. Prefer clarity to conciseness, so float_abs is better than the stdlib approach of fabs.
- In case of function overloading, write the argument postfix, e.g. a function that tests if a point is in a world may be written as world_contains_point.
- Mark all non-mutated parameters as const.
- Prefer returning over passing an output pointer.
- Function names should be actions or queries. (list_is_empty, list_clear)
- Construction functions should be called 'new' (list_new)
- Destruction functions should be called 'destroy' (list_destroy)

##Variables

- Global variables should be avoided, where possible. Prefer static variables in methods to globals.
- Variables should be preferrably one word, but if more than one should be snake_case
- Variables should be declared close to their use and in as tight of a scope as possible.
- No hungarian notation or type information should be attached to a variable name.
- Exception in cases of operands or indices, generally avoid single-letter names.

##Includes

- In an implementation file, the corresponding header include should be the first line, followed by a newline.
- C standard library includes should follow alphabetically, followed by a newline.
- System libraries should follow alphabetically, followed by a newline.
- Dependency headers should follow alphabetically, followed by a newline.
- Local headers should follow alphabetically, followed by a newline.

##Private function declarations

- Private function declarations should be placed just after the includes, in the order they appear in the function.

##Braces

- Opening braces should not be given their own line. Closing braces should, except in the case of 'else'. 
- In the case of an array or object initializer, prefer to keep the entire initializer on the same line.

##Initializers

- Array initializers should be used where possible.
- Object initializers should be used where possible.
- When passing non-named values to an object initializer, use the C99 .name = value syntax.
##Indentation
- Only indent to indicate a new lexical scope.
- Labels and switch 'case' statements should be set back an indent level.
- Indent and align with tab characters. 
- Tabs should be used as four characters wide.
