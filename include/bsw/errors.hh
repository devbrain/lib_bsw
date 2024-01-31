#ifndef BSW_ERRORS_HH
#define BSW_ERRORS_HH

#include "bsw/macros.hh"

#include <bsw/export.h>

#if defined(_WIN32) || defined(_WIN64)
#define _dERRORS_OS_WINDOWS
#elif defined(linux) || defined(__linux__) || defined(__linux)
#define _dERRORS_OS_LINUX
#else
#error "Unknown OS"
#endif

#include <string>

namespace bsw {
  using errno_type = int;
  BSW_EXPORT std::string error_to_string (errno_type err);
}

#endif