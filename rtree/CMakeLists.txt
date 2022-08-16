cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_BUILD_TYPE Debug)

project(rtree)

add_executable(rtree rational.cc main.cc rtree.cc)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(rtree_test rtree_test.cc rational.cc rtree.cc)

target_link_libraries(
  rtree_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(rtree_test)
