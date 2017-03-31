#ifndef BPUTIL_UTIL_HPP
#define BPUTIL_UTIL_HPP

#include <type_traits>
#include <cstddef>
#include <sstream>

#ifndef TESTING
#include "Particle.h"
#endif
#if HAS_EXCEPTIONS
#include <stdexcept>
#endif

#if defined(__has_include) && __has_include(<string_view>)
#define USE_STD_STRING_VIEW
#include <string_view>
#endif

namespace bp {
    template<typename T, typename U, typename... Us>
    struct is_any_of : std::integral_constant<bool,
            std::conditional<std::is_same<T, U>::value, std::true_type, is_any_of<T, Us...>>::type::value> {
    };

    template<typename T, typename U>
    struct is_any_of<T, U> : std::is_same<T, U>::type {
    };

    template <typename Derived, typename Base>
    using enable_if_instance = std::enable_if<std::is_base_of<Base, Derived>::value>;

    template<typename T, typename U, typename... Us>
    struct is_convertible_to : std::integral_constant<bool,
            std::conditional<std::is_convertible<T, U>::value, std::true_type, is_convertible_to<T, Us...>>::type::value> {
    };

    template<typename T, typename U>
    struct is_convertible_to<T, U> : std::is_convertible<T, U>::type {
    };


    template <typename T, typename U, typename... Us>
    using enable_if_convertible_to = typename std::enable_if<is_convertible_to<T, U, Us...>::value>::type;


    constexpr size_t const_length(const char *str) {
        return (*str == 0) ? 0 : const_length(str + 1) + 1;
    }

#if USE_STD_STRING_VIEW
    using string_view = std::string_view;
#else
    class string_view {
    public:
        constexpr string_view(const char* _str, const size_t _len)
                : p(_str), sz(_len) {}

        constexpr string_view(const char* _str)
                : p(_str), sz(const_length(_str)) {}

        string_view(const string_view &_s) = default;
        string_view(const std::string &_s) : p(_s.data()), sz(_s.size()) {}

        operator std::string();
        operator const std::string() const;

        constexpr string_view& operator=(const char* _str) {
            p = _str;
            sz = const_length(_str);
            return *this;
        }

        constexpr char operator[](std::size_t n) const {
            return n < sz ? p[n] :
                   #if HAS_EXCEPTIONS
                   throw std::out_of_range("")
                   #else
                   '\0'
                   #endif
                    ;
        }

        constexpr const char * data() const { return p; }

        constexpr std::size_t size() const { return sz; }

    private:
        const char *p;
        std::size_t sz;
    };

    namespace literals {
        string_view constexpr operator ""_sv(const char *_s, const size_t _len) {
            return bp::string_view(_s, _len);
        }
    }

#endif

    class blocked_string_view: public string_view {
    public:
        constexpr blocked_string_view(const char* a, const size_t _len) : string_view(a, _len) {}

        constexpr uint32_t get_block(int idx) {
            size_t i = (block_size() + idx) * 4;
            uint32_t b0 = data()[i];
            uint32_t b1 = data()[i + 1];
            uint32_t b2 = data()[i + 2];
            uint32_t b3 = data()[i + 3];
            return (b3 << 24) | (b2 << 16) | (b1 << 8) | b0;
        }

        constexpr std::size_t block_size() const { return size() / 4; }

        constexpr char tail(const int n) const {
            size_t tail_size = size() % 4;
            return data()[size() - tail_size + n];
        }
    };

    template <typename T>
    std::string to_string(T _val) {
#ifndef TESTING
        return std::string(String(_val).c_str());
#else
            return std::to_string(_val);
#endif
    }
}
#endif //BPUTIL_UTIL_HPP
