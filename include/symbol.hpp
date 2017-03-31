#ifndef BPUTIL_SYMBOL_HPP
#define BPUTIL_SYMBOL_HPP
#include <cstddef>
#include <cstdint>
#include <string>
#include <cstring>
#include <map>
#include "util.hpp"

#if __cpp_constexpr >= 201304L
#define CPP14_CONSTEXPR
#include "murmur3.hpp"
#endif

#include <memory>

namespace bp {
//    using hash_type = uint32_t;

    class hash_type;
//    constexpr hash_type hash(const char *_str, const size_t _len = 0);
    constexpr hash_type hash(const bp::string_view &_str);

    class hash_type {
    public:
        using type = uint32_t;
        explicit constexpr hash_type(type _hash=hash("")):hash_(_hash) {}
        constexpr hash_type(const hash_type &_hash):hash_(_hash) {}
        constexpr hash_type& operator=(const hash_type &_hash){hash_=_hash;return *this;}

        constexpr operator type() const { return hash_; }

    private:
        /*const*/ type hash_;
    };
    inline bool operator<(const hash_type &_a, const hash_type &_b) { return static_cast<hash_type::type>(_a)< static_cast<hash_type::type>(_b);}

    namespace {

#ifdef CPP14_CONSTEXPR
        constexpr hash_type::type murmur3_hash(const char *data, const size_t _len) {
            return mm3_x86_32(blocked_string_view(data, _len), 0x9747b28c);
        }
#else
        // djb2 hash
        const hash_type::type HASH_INITIALIZER = 5381;
        constexpr hash_type::type djb2_recursive(const hash_type::type &_hash, const char *_str) {
            return (!*_str ? _hash : djb2_recursive(((_hash << 5) + _hash) + *_str, _str + 1));
        }

        constexpr hash_type djb2_hash(const char* _str, const size_t) { return djb2_recursive(HASH_INITIALIZER, _str); };
#endif

    }

    constexpr hash_type hash(const bp::string_view &_str) {
#ifdef CPP14_CONSTEXPR
        return hash_type(murmur3_hash(_str.data(), _str.size()));
#else
        return hash_type(djb2_hash(_str.data(), _str.size()));
#endif
    };

    class symbol {
        using sym_map = std::map<bp::hash_type::type, std::string>;
    public:

        symbol();
        symbol(const symbol&) = default;
        symbol(symbol&&) = default;
        symbol& operator=(const symbol& _s) = default;
        symbol& operator=(symbol&& _s) = default;

        symbol(const bp::string_view &_s);

        inline bp::string_view to_string() const { return symbol::name(hash_); }
        inline bp::hash_type to_hash() const { return hash_; }
        inline explicit operator bp::hash_type () const { return hash_; }
        inline bool operator < (const symbol& _s) { return hash_<_s.hash_; }

        static void define(std::initializer_list<const char*>);
        static bp::string_view name(const bp::hash_type &_hash);

    private:
        static std::shared_ptr<sym_map> get_map();
        hash_type hash_;
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


    inline bool operator==(const symbol &_s1, const symbol &_s2){return static_cast<hash_type>(_s1) == static_cast<hash_type>(_s2);}
    inline bool operator!=(const symbol &_s1, const symbol &_s2){return static_cast<hash_type>(_s1) != static_cast<hash_type>(_s2);}
    inline bool operator==(const symbol &_s1, hash_type _s2){return static_cast<hash_type>(_s1) == _s2;}
    inline bool operator!=(const symbol &_s1, hash_type _s2){return static_cast<hash_type>(_s1) != _s2;}
    inline bool operator==(hash_type _s1, const symbol &_s2){return _s1 == static_cast<hash_type>(_s2);}
    inline bool operator!=(hash_type _s1, const symbol &_s2){return _s1 != static_cast<hash_type>(_s2);}

    namespace literals {
        hash_type constexpr operator ""_h(const char *_s, const size_t _len) {
            return hash(bp::string_view(_s, _len));
        }
    }
}

namespace std {
    template <>
    struct hash<bp::symbol>
    {
        std::size_t operator()(const bp::symbol & k) const
        {
            return k.to_hash();
        }
    };

    template <>
    struct hash<bp::hash_type>
    {
        std::size_t operator()(const bp::hash_type & k) const
        {
            return static_cast<bp::hash_type::type>(k);
        }
    };
}
#endif //BPUTIL_SYMBOL_HPP
