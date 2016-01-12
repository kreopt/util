cmake_minimum_required(VERSION 3.2)
project(bputil)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES include/symbol.hpp include/os.hpp include/log.hpp src/util.cpp)
add_library(bputil SHARED ${SOURCE_FILES})

install(TARGETS bputil DESTINATION lib/binelpro/)
install(DIRECTORY include/ DESTINATION include/binelpro/ FILES_MATCHING PATTERN "*.hpp")