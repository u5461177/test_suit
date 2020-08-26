cmake_minimum_required(VERSION 3.12)
project(testthreadpol)

set(CMAKE_CXX_STANDARD 14)

file(GLOB FILES *.h *.cpp)

add_executable(testthreadpol ${FILES})
target_link_libraries(testthreadpol pthread)