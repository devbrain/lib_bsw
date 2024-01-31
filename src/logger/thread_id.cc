#if defined(_MSC_VER)
#include <Windows.h>
#else
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#if defined(__APPLE__)
#include <sys/syscall.h>
#endif
#endif
#include "bsw/logger/thread_id.hh"


#if defined(PREDEF_COMPILER_MINGW32)
#include <iostream>
#endif


namespace bsw::mt
{
    thread_id_t current_thread_id()
    {
#if defined(_MSC_VER)      
        return GetCurrentThreadId();
#elif defined(__APPLE__)
        return static_cast <thread_id_t>(syscall(SYS_thread_selfid));
#else
        return static_cast <thread_id_t>(pthread_self());
#endif      
    }

    int get_current_pid()
    {
#if defined(_MSC_VER)
        return GetCurrentProcessId();
#else
        return getpid();
#endif
    }

} // ns mt


#if defined(PREDEF_COMPILER_MINGW32)
std::ostream& operator << (std::ostream& os, const ptw32_handle_t& x)
{
    os << x.p;
    return os;
}
#endif
