cmake_minimum_required(VERSION 3.3)
project(FlocosSandbox)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors -Wall -Wextra")

add_subdirectory(FlocosEngine)
add_subdirectory(Sandbox)

