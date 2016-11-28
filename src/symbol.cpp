#include "symbol.hpp"
#include <map>

static std::map<bp::hash_type, std::string> symbols;

bp::hash_type bp::hash(const std::string &_str) {
    return hash(_str.c_str());
}

void bp::__define_symbol(const char *_sym) {
    symbols.emplace(static_cast<hash_type >(bp::hash(_sym)), std::string(_sym));
}
void bp::__define_symbol(std::string &&_s) {
    symbols.emplace(static_cast<hash_type >(bp::hash(_s.c_str())), _s);
}

void bp::define_symbols(std::initializer_list<const char *> _l) {
    for (const auto &s: _l) {
        define_symbol<const char*>(s);
    }
}

std::string bp::sym_name(const bp::hash_type &_hash){
    auto hash = static_cast<bp::hash_type>(_hash);
    if (symbols.count(hash)) {
        return symbols.at(hash);
    } else {
        return bp::to_string(hash);
    }
}

bp::symbol::symbol(const std::string &_s) : hash_(hash(_s.c_str())){
    __define_symbol(_s.c_str());
}

bp::symbol::symbol(std::string &&_s) : hash_(hash(_s.c_str())){
    __define_symbol(std::forward<std::string>(_s));
}

bp::symbol::symbol() :hash_(hash("")){}

bp::symbol::symbol(const char *_s, const size_t _len) : hash_(hash(_s)) {
    __define_symbol(_s);
}

namespace bp {
    template<>
    void define_symbol(const char *_h) {
        __define_symbol(_h);
    }
}