#ifndef BPUTIL_SYMBOL_HPP
#define BPUTIL_SYMBOL_HPP
#include <cstddef>
#include <cstdint>
#include <string>
#include <cstring>
#include "util.hpp"

#if __cpp_constexpr > 201304L
#include "murmur3.hpp"
#endif

namespace bp {
    using hash_type = uint32_t;

    namespace {

#if __cpp_constexpr > 201304L
        constexpr symbol murmur3_hash(const char *data, const size_t _len) {
            return mm3_x86_32(str_view(data, _len), 0x9747b28c);
        }
#else
        // djb2 hash
        const hash_type HASH_INITIALIZER = 5381;
        constexpr hash_type djb2_recursive(const unsigned int _hash, const char *_str) {
            return (!*_str ? _hash : djb2_recursive(((_hash << 5) + _hash) + *_str, _str + 1));
        }

        constexpr hash_type djb2_hash(const char* _str, const size_t) { return djb2_recursive(HASH_INITIALIZER, _str); };
#endif

    }

    constexpr hash_type hash(const char *_str, const size_t _len = 0){
#if __cpp_constexpr > 201304L
        return murmur3_hash(_str, _len?_len:const_length(_str));
#else
        return djb2_hash(_str, _len?_len:const_length(_str));
#endif
    };
    hash_type hash(const std::string &_str);;

    class symbol;

    void __define_symbol(const char *_sym);
    void __define_symbol(std::string &&_s);

    void define_symbols(std::initializer_list<const char*> _l);

    std::string sym_name(const bp::hash_type &_hash);
//    inline std::string sym_name(symbol _hash){ return sym_name(static_cast<hash_type>(_hash));};

    class symbol {
        hash_type hash_;
    public:

        symbol();
        explicit symbol(const char* _s, const size_t _len=0);
        symbol(const symbol&) = default;
        symbol(symbol&&) = default;
        symbol& operator=(const symbol& _s) = default;
        symbol& operator=(symbol&& _s) = default;

        symbol(const std::string &_s);
        symbol(std::string &&_s);

        inline std::string to_string() const { return sym_name(hash_); }
        inline bp::hash_type to_hash() const { return hash_; }
        inline explicit operator bp::hash_type () const { return hash_; }
        inline bool operator < (const symbol& _s) { return hash_<_s.hash_; }
    };

    template <typename T>
    constexpr inline bool __is_hashable(){ return std::is_convertible<T, symbol>::value;}

    template <>
    constexpr inline bool __is_hashable<const char *>(){ return true;}

    template <>
    constexpr inline bool __is_hashable<char *>(){ return true;}

    template <typename T>
    struct is_hashable: public std::integral_constant<bool, __is_hashable<T>()>{
    };

    template <typename T>
    using enable_if_hashable_t = typename std::enable_if<bp::is_hashable<typename std::decay<T>::type>::value>::type;

    template <typename T>
    inline void define_symbol(T) {}

    template <>
    inline void define_symbol<const char *>(const char *_h);


    inline bool operator==(const symbol &_s1, const symbol &_s2){return static_cast<hash_type>(_s1) == static_cast<hash_type>(_s2);}
    inline bool operator!=(const symbol &_s1, const symbol &_s2){return static_cast<hash_type>(_s1) != static_cast<hash_type>(_s2);}
    inline bool operator==(const symbol &_s1, hash_type _s2){return static_cast<hash_type>(_s1) == _s2;}
    inline bool operator!=(const symbol &_s1, hash_type _s2){return static_cast<hash_type>(_s1) != _s2;}
    inline bool operator==(hash_type _s1, const symbol &_s2){return _s1 == static_cast<hash_type>(_s2);}
    inline bool operator!=(hash_type _s1, const symbol &_s2){return _s1 != static_cast<hash_type>(_s2);}

    namespace literals {
        hash_type constexpr operator ""_h(const char *_s, const size_t _len) {
            return hash(_s, _len);
        }
    }
}

namespace std {
    template <>
    struct hash<bp::symbol>
    {
        std::size_t operator()(const bp::symbol & k) const
        {
            return static_cast<bp::hash_type>(k);
        }
    };
}
#endif //BPUTIL_SYMBOL_HPP
