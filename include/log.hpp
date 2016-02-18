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
        enum class log_type: char {
            none = '-',
            error = 'E',
            info = 'I',
            warn = 'W',
            debug ='D',
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

            switch (_type) {
                case log_type::error:
                    std::cerr <<"["<< static_cast<char>(_type) <<"] "<< _s << std::endl;
                default:
                    std::cout <<"["<< static_cast<char>(_type) <<"] "<< _s << std::endl;
            }
#endif
        }
        inline static void _tagged_log(const char* _tag, const char *_s, log_type _type) {
            _log(std::string("==").append(_tag).append("==: ").append(_s).c_str(), _type);
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

        inline static void d(const char *_tag, const char* _s) {
            _tagged_log(_tag, _s, log_type::debug);
        }
        inline static void w(const char *_tag, const char* _s) {
            _tagged_log(_tag, _s, log_type::warn);
        }
        inline static void i(const char *_tag, const char* _s) {
            _tagged_log(_tag, _s, log_type::info);
        }
        inline static void e(const char *_tag, const char* _s) {
            _tagged_log(_tag, _s, log_type::error);
        }

        inline static void d(const std::string &_s) { Log::d(_s.c_str());}
        inline static void w(const std::string &_s) { Log::w(_s.c_str());}
        inline static void i(const std::string &_s) { Log::i(_s.c_str());}
        inline static void e(const std::string &_s) { Log::e(_s.c_str());}

        inline static void d(const char *_tag, const std::string &_s) { Log::d(_tag, _s.c_str());}
        inline static void w(const char *_tag, const std::string &_s) { Log::w(_tag, _s.c_str());}
        inline static void i(const char *_tag, const std::string &_s) { Log::i(_tag, _s.c_str());}
        inline static void e(const char *_tag, const std::string &_s) { Log::e(_tag, _s.c_str());}
    };
}
#endif //INTERPROCESS_UTIL_HPP
