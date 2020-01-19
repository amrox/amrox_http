cmake_minimum_required(VERSION 3.14.0)
project(amrox_http)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

find_package(Boost 1.71.0 REQUIRED COMPONENTS filesystem)
find_package(docopt.cpp REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

#add_library(amrox_http)

add_executable(amrox_http src/main.cpp src/request.hpp)
target_link_libraries(amrox_http
        PRIVATE
        Boost::Boost
        docopt.cpp::docopt.cpp
        spdlog::spdlog)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
