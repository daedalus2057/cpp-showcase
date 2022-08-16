cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_BUILD_TYPE Debug)

project(scorp)

add_executable(scorp main.cc scorp.cc deck.cc board.cc player.cc)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(scorp_test scorp_test.cc scorp.cc deck.cc board.cc player.cc)

target_link_libraries(
  scorp_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(scorp_test)
