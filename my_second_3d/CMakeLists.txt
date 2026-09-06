cmake_minimum_required(VERSION 3.16)

project(Triangle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Triangle
    src/main.cpp
    src/gl.c
)

target_include_directories(Triangle PRIVATE
    src/include
)

target_link_libraries(Triangle
    glfw
    GL
    dl
    pthread
)