cmake_minimum_required(VERSION 3.2)
project(bputil)
include(cmake/ExtLibs.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(BPU_VERSION_MAJOR 1)
set(BPU_VERSION_MINOR 0)
set(BPU_VERSION_PATCH 0)
set(BPU_VERSION ${BPU_VERSION_MAJOR}.${BPU_VERSION_MINOR}.${BPU_VERSION_PATCH} )

set(SOURCE_FILES include/symbol.hpp include/os.hpp include/log.hpp src/util.cpp include/util.hpp include/background_process.hpp)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${BPU_VERSION} SOVERSION ${BPU_VERSION_MAJOR} )

set(CMAKECONFIG_INSTALL_DIR "lib/cmake/bputil")

install(TARGETS bputil DESTINATION lib/binelpro/)
install(DIRECTORY include/ DESTINATION include/binelpro/ FILES_MATCHING PATTERN "*.hpp")
install(FILES "cmake/BpUtilConfig.cmake" DESTINATION ${CMAKECONFIG_INSTALL_DIR})
install(FILES "cmake/BpUtilConfigVersion.cmake" DESTINATION ${CMAKECONFIG_INSTALL_DIR})
install(FILES "cmake/ExtLibs.cmake" DESTINATION ${CMAKECONFIG_INSTALL_DIR})