#ifndef BPUTIL_UTIL_HPP
#define BPUTIL_UTIL_HPP

#include <type_traits>
#include <cstddef>
#include <sstream>


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

    template <typename T>
    std::string to_string(T _val) {
        return std::to_string(_val);
    }
}
#endif //BPUTIL_UTIL_HPP
