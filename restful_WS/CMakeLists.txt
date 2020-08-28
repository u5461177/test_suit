cmake_minimum_required(VERSION 3.12)
project(restfultest)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
link_directories(lib)

add_executable(restfultest main.cpp)
target_link_libraries(restfultest  crypto restbed)