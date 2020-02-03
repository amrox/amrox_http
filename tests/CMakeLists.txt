include(GoogleTest)

find_package(GTest)

add_executable(amrox_http_tests tests.cpp)
target_link_libraries(amrox_http_tests GTest::GTest amrox_http_lib)
#target_include_directories(amrox_http_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

gtest_discover_tests(amrox_http_tests)
# add_test(NAME monolithic COMMAND tests)