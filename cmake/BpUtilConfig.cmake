# - Try to find DCM IPC
# Once done this will define
#  BPUTIL_FOUND - System has DCM IPC
#  BPUTIL_INCLUDE_DIRS - The DCM IPC include directories
#  BPUTIL_LIBRARIES - The libraries needed to use DCM IPC
#  BPUTIL_DEFINITIONS - Compiler switches required for using DCM IPC

#set(BPUTIL_INSTALL_PREFIX "/usr/local")
set(CMAKECONFIG_INSTALL_DIR "lib/cmake/bputil")

find_path(BPUTIL_INCLUDE_DIRS
        NAMES binelpro/symbol.hpp
        DOC "binelpro include dir"
        PATHS /usr/local/include /usr/include
        )

find_library(
        BPUTIL_LIBRARIES
        NAMES bputil
        DOC "bputil library"
        PATH_SUFFIXES binelpro/
)
include(${BPUTIL_INCLUDE_DIRS}/../${CMAKECONFIG_INSTALL_DIR}/ExtLibs.cmake)