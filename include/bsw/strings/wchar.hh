//
// Created by igor on 27/05/2022.
//

#ifndef INCLUDE_BSW_STRINGS_WCHAR_HH
#define INCLUDE_BSW_STRINGS_WCHAR_HH

#include <bsw/export.h>
#include <string>
#include <vector>

namespace bsw {
  BSW_EXPORT std::wstring utf8_to_wstring (const std::string& str);
  BSW_EXPORT std::string wstring_to_utf8 (const std::wstring& str);

  BSW_EXPORT std::wstring utf8_to_wstring(const char* const str);
  BSW_EXPORT std::string wstring_to_utf8(const wchar_t* const str);

  BSW_EXPORT void utf8_to_wstring(const std::string& str, std::vector <wchar_t>& out);
  BSW_EXPORT void wstring_to_utf8(const std::wstring& str, std::vector <char>& out);

  BSW_EXPORT void utf8_to_wstring(const std::vector <char>& str, std::vector <wchar_t>& out);
  BSW_EXPORT void wstring_to_utf8(const std::vector <wchar_t>& str, std::vector <char>& out);

  BSW_EXPORT std::wstring utf8_to_wstring(const char* const str);
  BSW_EXPORT std::string wstring_to_utf8(const wchar_t* const str);

  BSW_EXPORT void utf8_to_wstring(const char* const str, std::vector <wchar_t>& out);
  BSW_EXPORT void wstring_to_utf8(const wchar_t* const str, std::vector <char>& out);
}

#endif //INCLUDE_BSW_STRINGS_WCHAR_HH
