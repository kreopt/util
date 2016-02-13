#ifndef BPUTIL_SYMBOL_HPP
#define BPUTIL_SYMBOL_HPP
#include <cstddef>
#include <cstdint>
#include <string>
#include <cstring>
#include <iostream>

namespace bp {
    namespace {
        using _hash_type = uint64_t ;
        const _hash_type HASH_INITIALIZER = 5381;
        constexpr _hash_type symbol_recursive(unsigned int hash, const char *str) {
            return (!*str ? hash : symbol_recursive(((hash << 5) + hash) + *str, str + 1));
        }
    }

    struct symbol {
        using hash_type = _hash_type ;
        // TODO: getters for hash and name. deny to set it manually
        hash_type hash = 0;
        std::string name;
        symbol(const char* _name) : hash(symbol_recursive(HASH_INITIALIZER, _name)), name(_name) {}
        symbol(const std::string &_name) : symbol(_name.c_str()){}
        symbol(hash_type _hash, const char* name="") : hash(_hash), name(name) {}
        symbol(const symbol & _sym) : hash(_sym.hash), name(_sym.name) {}
        symbol(symbol && _sym) : hash(std::move(_sym.hash)), name(std::move(_sym.name)) {}

        inline operator hash_type() const { return hash;}
        inline operator const char*() const { return name.c_str();}
        inline operator const std::string() const { return name;}
        inline bool valid() const { return hash!=HASH_INITIALIZER; }

        bp::symbol& operator=(const bp::symbol& _sym) {
            this->hash = _sym.hash;
            this->name = _sym.name;
            return *this;
        }
    };

    constexpr symbol::hash_type hash(const char* _str) { return symbol_recursive(HASH_INITIALIZER, _str); };

    inline bool operator==(const symbol &_s1, const symbol &_s2){return _s1.hash == _s2.hash;}

    namespace literals {
        inline symbol operator ""_sym(const char *s, size_t) {return bp::symbol(s);};
        symbol::hash_type constexpr operator ""_hash(const char *s, size_t) {
            return symbol_recursive(HASH_INITIALIZER, s);
        }
    }
}

namespace std {
    template <>
    struct hash<bp::symbol>
    {
        std::size_t operator()(const bp::symbol & k) const
        {
            return k.hash;
        }
    };
}
#endif //BPUTIL_SYMBOL_HPP
