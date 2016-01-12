#ifndef BPUTIL_SYMBOL_HPP
#define BPUTIL_SYMBOL_HPP
#include <cstddef>
#include <cstdint>
#include <string>
#include <iostream>

namespace bp {
    // TODO: store string info in type
    using symbol_t = uint64_t ;

    namespace {
        constexpr uint64_t symbol_recursive(unsigned int hash, const char *str) {
            return (!*str ? hash : symbol_recursive(((hash << 5) + hash) + *str, str + 1));
        }
    }

    constexpr uint64_t symbol(const char *str) {
        return (!str ? 0 : symbol_recursive(5381, str));
    }

    symbol_t constexpr operator ""_sym(const char *s, size_t) {
        return symbol(s);
    }

    extern uint64_t symbol(const std::string &str);
}

#endif //BPUTIL_SYMBOL_HPP
