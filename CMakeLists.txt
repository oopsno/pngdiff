cmake_minimum_required(VERSION 3.5)
project(pngdiff)

set(CMAKE_CXX_STANDARD 14)

find_package(PNG REQUIRED)
include_directories(include ${PNG_INCLUDE_DIRS})

file(GLOB PNGDIFF_SOURCES src/*.cpp)
add_library(png-diff ${PNGDIFF_SOURCES})
target_link_libraries(png-diff ${PNG_LIBRARIES})

add_executable(pngdiff main.cpp)
target_link_libraries(pngdiff png-diff)
