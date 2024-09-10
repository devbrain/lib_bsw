//
// Created by igor on 9/10/24.
//

#ifndef BSW_JSON_HH
#define BSW_JSON_HH

#include <stdexcept>
#include <bsw/export.h>

#if !defined(JSON_DIAGNOSTICS)
#define JSON_DIAGNOSTICS 1
#endif

#if defined(JSON_NOEXCEPTION)
#undef JSON_NOEXCEPTION
#endif

#if !defined(JSON_ASSERT)
namespace bsw {
    [[noreturn]] BSW_EXPORT void json_assert_handler(const char* x, const char* file, int line);
    [[noreturn]] BSW_EXPORT void json_raise_exception(const std::exception& e);
}

#define JSON_ASSERT(cond) ((cond) ? \
(void)0 : \
(void)bsw::json_assert_handler(#cond, __FILE__, __LINE__))
#endif


#define JSON_THROW_USER bsw::json_raise_exception

#include <nlohmann/json.hpp>

#endif
