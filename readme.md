# Pride

> **General purpose C++17 core utility header-only library**

Pride is written to be my core utility library that use in most of my projects. Is is written to be a header only library so that it is easy to include and use. Pride is ment to be added as a submodule and added to your cmake files.

## Install / Usage

```bash
$ mkdir external
$ git submodule add https://github.com/cruizemissile/pride external/pride
```
```cmake
add_subdirectory(external/pride)
add_executable(sample)
target_link_libraries(sample ${PRIDE_LIB})
```

## Features:

- Detection for architecture, compiler, operating system, c++ version, endian-ness, debug, rtti, and stdlib
- Static and dynamic api export macros
- Preprocessor helper utilities
- Cross platform console color output and terminal utilities
- Assert
- Guid
- Singleton
- Scope Guards and defer macro
- Type defines

### Compile Time Features

- **`pride::ct::nameof<T>()`** : `constexpr` demangled full qualified type name
- **`pride::ct::nameof<T, T Value>()`** : `constexpr` demangled value type
- **`pride::ct::type_id<T>()`** : `constexpr` `std::type_info` - like type id
- **`pride::ct::unnamed_type_id<T>()`** : `constexpr` `std::type_info` - like hash-only type id
- **`pride::ct::hash::fnv1a("")`** : `constexpr` hash c style string  and cstring_t
- **`pride::ct::cstring_t`** : `constexpr` c style compile time string
- **`pride::ct::static_value<T, Value>`** : `constexpr` way to pass values as template parameters
- **`pride::ct::type_list<...>`** : `constexpr` compile time way to store list of types 

###  Todo

- [ ] Logging
- [ ] Profiling
- [ ] Custom allocators

## Development disclammer
This project is currently under development and is only updated when I have some free time.
