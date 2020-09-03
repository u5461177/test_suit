cmake_minimum_required(VERSION 3.10)
project(Algoapi)

set(CMAKE_CXX_STANDARD 17)

link_directories(/usr/local/lib64/)
link_directories(/usr/local/lib)
include_directories(/usr/local/include)

#find_package(CURL REQUIRED)
#include_directories(${CURL_INCLUDE_DIRS})


find_package (Boost 1.62.0 REQUIRED COMPONENTS system thread)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

find_package(OpenSSL)
INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})


include_directories(./include)


add_library(algoapi SHARED AlgoapiOri.cpp include/taskunit.h taskunit.cpp Algodatatype.cpp include/libalgo.h)
TARGET_LINK_LIBRARIES(algoapi curlpp OpenSSL::SSL OpenSSL::Crypto ${Boost_LIBRARIES} curl pthread)


add_library(algospi SHARED AlgospiSimple.cpp)
add_executable(main main.cpp)
TARGET_LINK_LIBRARIES(main algoapi algospi)
