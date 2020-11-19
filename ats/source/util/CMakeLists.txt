cmake_minimum_required(VERSION 3.12)
project(util)
set(CMAKE_CXX_STANDARD 17)

include_directories(../../include)
link_directories(../../lib/linux)


file(GLOB SOURSE_FILES *.h *.cpp)

add_library(util SHARED ${SOURSE_FILES})
target_link_libraries(util glog)