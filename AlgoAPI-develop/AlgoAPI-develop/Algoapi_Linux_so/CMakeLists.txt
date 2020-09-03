cmake_minimum_required(VERSION 3.10)
project(main)

set(CMAKE_CXX_STANDARD 17)


link_directories(./)
include_directories(./include)

add_library(algospi SHARED AlgospiSimple.cpp)
add_executable(main main.cpp)
TARGET_LINK_LIBRARIES(main algoapi algospi curlpp)
