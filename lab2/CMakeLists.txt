cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_BUILD_TYPE Debug)

project(deter)

add_executable(deter main.cc deter.cc)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(deter_test deter_test.cc deter.cc)

target_link_libraries(
  deter_test
  gtest_main
)

include(GoogleTest)
#gtest_discover_tests(deter)
