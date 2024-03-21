cmake_minimum_required(VERSION 3.12)
project(Practical)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES 
    src/main.cpp
    src/filter.cpp
    src/Image.cpp
    src/volume.cpp
    src/slice.cpp
    src/projection.cpp
    src/utility.cpp
)
add_executable(image ${SOURCES})
