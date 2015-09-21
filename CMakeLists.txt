cmake_minimum_required(VERSION 3.3)
project(pyextension)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/foomodule.c src/foomodule.h)

include_directories($ENV{PY_INC_PATH})
add_executable(pyextension ${SOURCE_FILES})