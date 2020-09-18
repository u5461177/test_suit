cmake_minimum_required(VERSION 3.17)
project(timestamp)

set(CMAKE_CXX_STANDARD 14)
file(GLOB SOURCE_FILES *h, *cpp)
add_library(timestamp SHARED ${SOURCE_FILES})