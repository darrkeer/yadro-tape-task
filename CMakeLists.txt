cmake_minimum_required(VERSION 3.16)
project(yadro-tape-task)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(tape STATIC ./src/tape.h ./src/tape.cpp)
add_library(tape-utils STATIC ./src/tape-utils.h ./src/tape-utils.cpp)
add_executable(tape-test ./tests/tape-test.cpp)

target_link_libraries(tape tape-utils)
target_link_libraries(tape-test tape GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tape-test)