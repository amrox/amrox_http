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

add_library(amrox_http_lib
        src/connection.cpp
        src/connection_manager.cpp
        src/request_parser.cpp 
        src/server.cpp
        src/session.cpp)


target_include_directories(amrox_http_lib
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_BINARY_DIR}/gen-private-include
)

target_link_libraries(amrox_http_lib
        PUBLIC
        Boost::Boost
        spdlog::spdlog)

add_executable(amrox_http
        src/main.cpp)

target_link_libraries(amrox_http
        PRIVATE
        docopt.cpp::docopt.cpp
        amrox_http_lib)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()
