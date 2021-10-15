cmake_minimum_required(VERSION 3.2)
project(cppregistry)

include_directories("include")

file(GLOB EXAMPLE_SRC examples/example1/*.cpp)

add_executable(example ${EXAMPLE_SRC})
