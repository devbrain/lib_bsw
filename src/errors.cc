#include "bsw/errors.hh"
#if defined(_dERRORS_OS_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <cerrno>
#endif

#include <sstream>

namespace bsw
{

	std::string error_to_string(errno_type err)
	{
#if defined(_dERRORS_OS_WINDOWS)
		std::string res;
		LPVOID lpMsgBuf;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)&lpMsgBuf,
			0, NULL);
		res.assign((char*)lpMsgBuf);
		LocalFree(lpMsgBuf);
		return res;
#else
        static constexpr std::size_t size = 1024;
        std::string res;
        res.resize(size);
        strerror_r(static_cast<int>(err), &res[0], size);
        return res;
#endif
	}

}
