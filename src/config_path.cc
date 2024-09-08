//
// Created by igor on 9/8/24.
//

#include <cstdlib>
#include <bsw/config_path.hh>
#include <bsw/whereami.hh>

#if defined(__linux__) || defined(BSD)
#define CFG_LINUX
#include <cstdlib>
#elif defined(WIN32)
#define CFG_WINDOWS
#include <shlobj.h>
#include <vector>
#include <bsw/strings/wchar.hh>
#   if !defined(MAX_PATH)
#       define MAX_PATH 512
#   endif
#elif defined(__APPLE__)
#define CFG_MAC
#include <CoreServices/CoreServices.h>
#include <sys/stat.h>
#define MAX_PATH PATH_MAX
#endif

static std::filesystem::path get_config_from_home() {
    const char* home = getenv("HOME");
    if (!home) {
        auto exe_path = bsw::get_executable_path();
        return exe_path.parent_path() / "config";
    }
    return std::filesystem::path(home) / ".config";
}

std::filesystem::path bsw::get_config_path() {
#if defined(CFG_LINUX)
    if (char* home = getenv("XDG_CONFIG_HOME")) {
        return {home};
    }
    return get_config_from_home();
#elif defined(CFG_WINDOWS)
    std::vector <wchar_t> out(MAX_PATH, 0);
    if (!SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, out.data()))) {
        return get_config_from_home();
    }
    return wstring_to_utf8(out.data());
#elif defined(CFG_MAC)
    FSRef ref;
    FSFindFolder(kUserDomain, kApplicationSupportFolderType, kCreateFolder, &ref);
    char home[MAX_PATH];
    FSRefMakePath(&ref, (UInt8 *)&home, MAX_PATH);
    return {home};
#endif
}
