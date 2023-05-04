#include <bsw/console.hh>

#if defined(_WIN32)
#define BUILD_WIN_CONSOLE
#endif

#if defined(BUILD_WIN_CONSOLE)

#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <windows.h>

static ULONG_PTR GetParentProcessId() // By Napalm @ NetCore2K
{
    ULONG_PTR pbi[6];
    ULONG ulSize = 0;
    static LONG(WINAPI * NtQueryInformationProcess)(HANDLE ProcessHandle, ULONG ProcessInformationClass,
        PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength) = nullptr;
    if (!NtQueryInformationProcess) {
        *(FARPROC*)&NtQueryInformationProcess =
            GetProcAddress(LoadLibraryA("NTDLL.DLL"), "NtQueryInformationProcess");
    }
    if (NtQueryInformationProcess) {
        if (NtQueryInformationProcess(GetCurrentProcess(), 0, &pbi, sizeof(pbi), &ulSize) >= 0 && ulSize == sizeof(pbi)) {
            return pbi[5];
        }
    }
    return (ULONG_PTR)-1;
}
#endif
namespace bsw {
	console::console() {
#if defined(BUILD_WIN_CONSOLE)
        if (GetConsoleWindow()) {
            return;
        }
        ULONG_PTR ppid = GetParentProcessId();
        if (ppid == (ULONG_PTR)-1) {
            AllocConsole();
        }
        else {
            AttachConsole((DWORD)ppid);
        }

        (void)freopen("CONIN$", "r", stdin);
        (void)freopen("CONOUT$", "w", stdout);
        (void)freopen("CONOUT$", "w", stderr);
#endif
	}

	console::~console() {
#if defined(BUILD_WIN_CONSOLE)
        if (GetConsoleWindow()) {
            return;
        }
        FreeConsole();
#endif
	}
}