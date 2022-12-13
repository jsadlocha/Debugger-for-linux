cmake_minimum_required(VERSION 3.16)

set(This DebugTest)

project(${This} C CXX)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)

FetchContent_MakeAvailable(googletest)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(${This} DebugTest.cpp)

target_link_libraries(${This} PRIVATE GTest::GTest DebugLib)

add_test(
  NAME ${This}
  COMMAND ${This}
)