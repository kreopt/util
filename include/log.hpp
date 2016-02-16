#ifndef INTERPROCESS_UTIL_HPP
#define INTERPROCESS_UTIL_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <iostream>

#ifdef USE_BOOST_LOG
#include <boost/log/trivial.hpp>
#endif

namespace bp {
    class Log {
        enum class log_type: uint {
            none = 0,
            error = 1,
            info = 2,
            warn = 3,
            debug =4,
        };


        inline static void _log(const char *_s, log_type _type) {
#ifdef USE_BOOST_LOG
            switch (_type) {
                case log_type::debug:BOOST_LOG_TRIVIAL(debug) << _s;break;
                case log_type::warn:BOOST_LOG_TRIVIAL(warning) << _s;break;
                case log_type::info:BOOST_LOG_TRIVIAL(info) << _s;break;
                case log_type::error:BOOST_LOG_TRIVIAL(error) << _s;break;
            }
#elif !defined(DISABLE_LOG)

            std::cout << _s << std::endl;
#endif
        }

        public:
// TODO: log levels
        inline static void d(const char* _s) {
            _log(_s, log_type::debug);
        }
        inline static void w(const char* _s) {
            _log(_s, log_type::warn);
        }
        inline static void i(const char* _s) {
            _log(_s, log_type::info);
        }
        inline static void e(const char* _s) {
            _log(_s, log_type::error);
        }

        inline static void d(const std::string &_s) { Log::d(_s.c_str());}
        inline static void w(const std::string &_s) { Log::w(_s.c_str());}
        inline static void i(const std::string &_s) { Log::i(_s.c_str());}
        inline static void e(const std::string &_s) { Log::e(_s.c_str());}
    };
}
#endif //INTERPROCESS_UTIL_HPP
