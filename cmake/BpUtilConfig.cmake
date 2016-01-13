# - Try to find DCM IPC
# Once done this will define
#  BPUTIL_FOUND - System has DCM IPC
#  BPUTIL_INCLUDE_DIRS - The DCM IPC include directories
#  BPUTIL_LIBRARIES - The libraries needed to use DCM IPC
#  BPUTIL_DEFINITIONS - Compiler switches required for using DCM IPC

set(BPUTIL_INSTALL_PREFIX "/usr/local")
set(CMAKECONFIG_INSTALL_DIR "lib/cmake/bputil")

find_path(BPUTIL_INCLUDE_DIR ${BPUTIL_INSTALL_PREFIX}/binelpro/symbol.hpp PATH_SUFFIXES binelpro )

include(${BPUTIL_INSTALL_PREFIX}/${CMAKECONFIG_INSTALL_DIR}/ExtLibs.cmake)

set(BPUTIL_INCLUDE_DIRS, ${BPUTIL_INCLUDE_DIR})
set(BPUTIL_LIBRARIES  ${BPUTIL_EXT_LIBRARIES} ${BPUTIL_INSTALL_PREFIX}/lib/binelpro/libbputil.so)

mark_as_advanced(BPUTIL_INCLUDE_DIR OS_SPECIFIC)
