cmake_minimum_required(VERSION 3.23)
project(memdb LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS "-Werror -Wall -Wextra -g -O0")

set(SOURCE_FILES
        src/main.cpp
        src/database/database.cpp
        src/database/table.cpp
        src/database/row.cpp
        src/database/column.cpp
        src/parser/parser.cpp
        src/command/command.cpp
        src/expression/expression.cpp
        src/cell/cell.cpp
        src/command/result.cpp
)

set(TEST_FILES
        tests/parser_test.cpp
        tests/select_test.cpp)

include_directories(src/)
include_directories("${CMAKE_SOURCE_DIR}/googletest/include")

add_executable(memdb ${SOURCE_FILES})

enable_testing()
find_package(GTest REQUIRED)

add_executable(run_tests ${TEST_FILES} ${SOURCE_FILES})

target_link_libraries(run_tests GTest::gtest GTest::gtest_main)

add_test(AllTests run_tests)
