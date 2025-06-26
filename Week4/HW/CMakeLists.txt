cmake_minimum_required(VERSION 3.10)
project(HW1Test)
set(CMAKE_CXX_STANDARD 17)

# GoogleTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(HW1_test HW1_test.cpp HW1.cpp read_files.cpp)
target_link_libraries(HW1_test ${GTEST_LIBRARIES} pthread)

# 通常実行用バイナリ
add_executable(HW1_main main.cpp HW1.cpp read_files.cpp)
