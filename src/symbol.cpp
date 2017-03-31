#include "symbol.hpp"
#include <map>

//bp::hash_type bp::hash(const std::string &_str) {
//    return hash(_str.c_str(), _str.size());
//}

bp::string_view bp::symbol::name(const bp::hash_type &_hash){
    auto hash = static_cast<bp::hash_type::type>(_hash);
    auto map = get_map();
    if (!map->count(hash)) {
        map->emplace(hash, bp::to_string(hash));
    }
    return map->at(hash);
}

//bp::symbol::symbol(const char *_s, const size_t _len) : hash_(hash(_s)) {
//    size_t len = _len?_len:const_length(_s);
//    get_map()->emplace(static_cast<hash_type::type>(hash_), std::string(_s, len));
//}
bp::symbol::symbol(const bp::string_view &_s) : hash_(hash(_s)){
    get_map()->operator[](static_cast<hash_type::type>(hash_)) = std::string(_s.data(), _s.size());
}

//bp::symbol::symbol(std::string &&_s) : hash_(hash(_s)){
//    get_map()->emplace(static_cast<hash_type::type>(hash_), std::move(_s));
//}

bp::symbol::symbol() :hash_(hash("")){}

void bp::symbol::define(std::initializer_list<const char *> _l) {
    for (const auto &s: _l) {
        bp::symbol _(s);
    }
}

std::shared_ptr<bp::symbol::sym_map> bp::symbol::get_map() {
    static std::shared_ptr<sym_map> symbols;
    if (!symbols) {
        symbols = std::shared_ptr<sym_map>(new sym_map());
    }
    return symbols;
}
