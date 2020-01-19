include(GoogleTest)

find_package(GTest)

add_executable(amrox_http_tests tests.cpp)
target_link_libraries(amrox_http_tests GTest::GTest)

gtest_discover_tests(amrox_http_tests)
add_test(NAME monolithic COMMAND tests)