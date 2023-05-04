#ifndef BSW_LOGGER_THREAD_ID_HH
#define BSW_LOGGER_THREAD_ID_HH

#include "bsw/predef.h"

#if defined(PREDEF_COMPILER_MINGW32)
#include <iosfwd>
#endif


namespace mt
{
    typedef int thread_id_t;
    thread_id_t current_thread_id();
    int get_current_pid();
} // ns mt


#if defined(PREDEF_COMPILER_MINGW32)
API std::ostream& operator << (std::ostream& os, const ptw32_handle_t& x);
#endif

#endif
