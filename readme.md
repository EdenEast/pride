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

## Build and Development

```bash
# clone pride and its submodules
git clone --recurse-submodules -j8 https://github.com/cruizemissile/pride && cd pride

# generate and build with cmake
cmake . -Bbuild && cmake --build build

# note: if you want to build with just the samples and skip tests and benchmarks run with NO_TEST on
cmake . -Bbuild -DNO_TESTS=ON && cmake --build build
```

## Features:

- Detection for architecture, compiler, operating system, c++ version, endian-ness, debug, rtti, and stdlib
- Static and dynamic api export macros
- Preprocessor helper utilities
- Cross platform console color output and terminal utilities
- Assert
- Command line options parser
- Guid
- Singleton
- Scope Guards and defer macro
- Type defines

### Compile Time Features

- type traits
- fnv1a hash function
- constexpr string
- static_value
- type_list

### Compiler support

Pride is built to be a c++17 library and with that come the requirement for newer compilers. Pride is supported on clang, gcc, and msvc.

- clang 5.0
- gcc 8.1.1
- msvc [need to check msvc version]

###  Todo

- [ ] Logging
- [ ] Profiling
- [ ] Custom allocators

## Development disclammer
This project is currently under development and is only updated when I have some free time.
