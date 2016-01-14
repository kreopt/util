#ifndef BPUTIL_UTIL_HPP
#define BPUTIL_UTIL_HPP
#include <type_traits>

namespace bp {
template<typename T, typename U, typename... Us>
    struct is_any_of : std::integral_constant<bool,
                    std::conditional<std::is_same<T,U>::value, std::true_type, is_any_of<T,Us...>>::type::value>
    { };

    template<typename T, typename U>
    struct is_any_of<T,U> : std::is_same<T,U>::type { };
}
#endif //BPUTIL_UTIL_HPP
