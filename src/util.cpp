#include "util.hpp"

bp::string_view::operator std::string() { return std::string(p, sz); }

bp::string_view::operator const std::string() const { return std::string(p, sz); }