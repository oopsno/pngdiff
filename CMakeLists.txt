cmake_minimum_required(VERSION 3.5)
project(pngdiff)

set(CMAKE_CXX_STANDARD 14)

find_package(PNG REQUIRED)
include_directories(include ${PNG_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp src/pndiff.cpp)
add_executable(pngdiff ${SOURCE_FILES})
target_link_libraries(pngdiff ${PNG_LIBRARIES})
