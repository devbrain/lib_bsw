//
// Created by igor on 9/10/24.
//

#include <bsw/exception.hh>
#include <bsw/json/json.hh>

namespace bsw {
    void json_assert_handler(const char* x, const char* file, int line) {
        raise_exception("JSON", file, line, x);
    }

    void json_raise_exception(const std::exception& e) {
        raise_exception("JSON", __FILE__, 0, e.what());
    }
}
