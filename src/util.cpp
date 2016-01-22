#include "../include/symbol.hpp"
#include "../include/os.hpp"
#if defined(__linux__) || defined(__APPLE__) || defined(__unix__)
#include <sys/types.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <process.h>
#endif

uint32_t bp::getmypid() {
#if defined(__linux__) || defined(__APPLE__)
    return getpid();
#elif defined(_WIN32)
    return _getpid();
    #else
    return 0; // This should not happen
#endif
}