# amrox_http

This is a learning project for modern C++ and related libraries. The goal is to build a simple http server, similar to [`python -m http.server`](https://docs.python.org/3/library/http.server.html).

## Status

So far, it's just a command line program that will echo back to you.

 - [x] Command line parsing
 - [x] Echo server
 - [x] Test harness
 - [ ] Dockerize build
 - [ ] Actual HTTP protocol

## Quick start

Install a modern C++ compiler, `cmake` and `conan`.

```
mkdir build
cd build
conan install .. --build=missing
cmake ..
cmake --build .
```

## Info

Language:

 - C++17

Dependency manager:

 - [conan](https://conan.io/)

Build system:

 - Modern [CMake](https://cmake.org/)

Libraries:

 - [docopt](https://github.com/docopt/docopt.cpp)
 - [Boost ASIO](https://www.boost.org/doc/libs/1_72_0/doc/html/boost_asio.html)
 - [Boost Filesystem](https://www.boost.org/doc/libs/1_72_0/libs/filesystem/doc/reference.html)
 - [fmt](https://github.com/fmtlib/fmt)
 - [spdlog](https://github.com/gabime/spdlog)

# License

MIT
