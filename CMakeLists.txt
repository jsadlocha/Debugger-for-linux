cmake_minimum_required(VERSION 3.16)


set(This, main)

project(${This} C CXX)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build")

enable_testing()

include_directories(DebugLib src)

add_library(DebugLib SHARED src/tools/debugger.cpp src/tools/debugger.hpp)

add_subdirectory(test)

add_executable(main main.cpp)

target_link_libraries(main DebugLib)

# http://dlib.net/examples/CMakeLists.txt.html