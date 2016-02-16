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
        constexpr _hash_type symbol_recursive(const unsigned int hash, const char *str) {
            return (!*str ? hash : symbol_recursive(((hash << 5) + hash) + *str, str + 1));
        }
    }

    struct symbol {
        using hash_type = _hash_type ;

        symbol(const char* _name) : hash_(symbol_recursive(HASH_INITIALIZER, _name)), name_(_name) {}
        symbol(const std::string &_name) : symbol(_name.c_str()){}
        [[ deprecated("this constructor should be removed") ]]
        symbol(const hash_type _hash, const char* _name="") : hash_(_hash), name_(_name) {}
        symbol(const symbol & _sym) : hash_(_sym.hash_), name_(_sym.name_) {}
        symbol(symbol && _sym) : hash_(std::move(_sym.hash_)), name_(std::move(_sym.name_)) {}

        inline const hash_type hash() const { return hash_; }
        inline const std::string name() const { return name_; }
        inline operator hash_type() const { return hash_;}
        inline operator const char*() const { return name_.c_str();}
        inline operator const std::string() const { return name_;}
        inline bool valid() const { return hash_!=HASH_INITIALIZER; }

        friend std::ostream &operator<<(std::ostream &os, const symbol &_sym) {
            return os << _sym.name();
        }

        bp::symbol& operator=(const bp::symbol& _sym) {
            this->hash_ = _sym.hash_;
            this->name_ = _sym.name_;
            return *this;
        }

        bp::symbol& operator=(bp::symbol&& _sym) {
            this->hash_ = std::move(_sym.hash_);
            this->name_ = std::move(_sym.name_);
            return *this;
        }

    private:
        hash_type hash_ = 0;
        std::string name_;
    };

    constexpr symbol::hash_type hash(const char* _str) { return symbol_recursive(HASH_INITIALIZER, _str); };

    inline bool operator==(const symbol &_s1, const symbol &_s2){return _s1.hash() == _s2.hash();}

    namespace literals {
        inline symbol operator ""_sym(const char *s, const size_t) {return bp::symbol(s);};
        symbol::hash_type constexpr operator ""_hash(const char *s, const size_t) {
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
            return k.hash();
        }
    };
}
#endif //BPUTIL_SYMBOL_HPP
