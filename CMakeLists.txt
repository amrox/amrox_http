 cmake_minimum_required(VERSION 3.14.0)
 project(amrox_http)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(amrox_http src/main.cpp)
target_link_libraries(amrox_http ${CONAN_LIBS})
