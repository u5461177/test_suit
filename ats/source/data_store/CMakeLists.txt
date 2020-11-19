cmake_minimum_required(VERSION 3.12)
project(data_store)
set(CMAKE_CXX_STANDARD 17)

include_directories(../../include)
link_directories(../../lib/linux)



file(GLOB SOURSE_FILES *.h *.cpp)

add_library(data_store SHARED ${SOURSE_FILES})
target_link_libraries(data_store mysqlclient cpp_redis)

