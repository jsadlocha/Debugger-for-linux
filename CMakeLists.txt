cmake_minimum_required(VERSION 3.16)
project(main)
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp)

add_library(DebugLib STATIC src/tools/debugger.cpp src/tools/debugger.hpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} DebugLib)

# http://dlib.net/examples/CMakeLists.txt.html