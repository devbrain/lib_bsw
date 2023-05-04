#ifndef BSW_DLFCN_HH
#define BSW_DLFCN_HH

#if !defined(WIN32)
#include <dlfcn.h>
#else
#include <windows.h>

#define RTLD_LAZY 0

inline void* dlopen(const wchar_t* path, int)
{
	return LoadLibraryW(path);
}

inline void* dlopen(const char* path, int)
{
	return LoadLibraryA(path);
}

inline void* dlsym(void* dll, const char* symbol)
{
	return GetProcAddress(reinterpret_cast<HMODULE>(dll), symbol);
}

inline void dlclose(void* dll)
{
	FreeLibrary(reinterpret_cast<HMODULE>(dll));
}
#endif

#endif
