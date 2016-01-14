cmake_minimum_required(VERSION 3.2)
project(bputil)
include(cmake/ExtLibs.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES include/symbol.hpp include/os.hpp include/log.hpp src/util.cpp include/util.hpp)
add_library(bputil SHARED ${SOURCE_FILES})

set(CMAKECONFIG_INSTALL_DIR "lib/cmake/bputil")

install(TARGETS bputil DESTINATION lib/binelpro/)
install(DIRECTORY include/ DESTINATION include/binelpro/ FILES_MATCHING PATTERN "*.hpp")
install(FILES "cmake/BpUtilConfig.cmake" DESTINATION ${CMAKECONFIG_INSTALL_DIR})
install(FILES "cmake/BpUtilVersion.cmake" DESTINATION ${CMAKECONFIG_INSTALL_DIR})
install(FILES "cmake/ExtLibs.cmake" DESTINATION ${CMAKECONFIG_INSTALL_DIR})