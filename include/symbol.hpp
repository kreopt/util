#ifndef BPUTIL_SYMBOL_HPP
#define BPUTIL_SYMBOL_HPP
#include <cstddef>
#include <cstdint>
#include <string>
#include <cstring>
#include <iostream>

namespace bp {
    // TODO: store string info in type
    // using symbol_t = uint64_t ;

    namespace {
        using _hash_type = uint64_t ;
        constexpr _hash_type symbol_recursive(unsigned int hash, const char *str) {
            return (!*str ? hash : symbol_recursive(((hash << 5) + hash) + *str, str + 1));
        }
    }

    struct symbol_t {
        using hash_type = _hash_type ;
        const hash_type hash = 0;
        const char* name= nullptr;
        // TODO: copy name dynamically?
        constexpr symbol_t(const char* _name) : name(_name), hash(symbol_recursive(5381, _name)) {}

        inline constexpr operator hash_type(){ return hash;}
        inline constexpr operator const char*(){ return name;}
        inline constexpr bool valid() { return hash!=0; }

    };

    inline constexpr bool operator==(const symbol_t &_s1, const symbol_t &_s2){return _s1.hash==_s2.hash;}

    constexpr symbol_t symbol(const char *str) {
        return symbol_t(str);
    }

    namespace literals {
        symbol_t constexpr operator ""_sym(const char *s, size_t) {
            return symbol(s);
        }
    }

    extern symbol_t symbol(const std::string &str);
}

namespace std {
    template <>
    struct hash<bp::symbol_t>
    {
        std::size_t operator()(const bp::symbol_t& k) const
        {
            return k.hash;
        }
    };
}
#endif //BPUTIL_SYMBOL_HPP
