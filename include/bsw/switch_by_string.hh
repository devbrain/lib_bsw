//
// Created by igor on 26/07/2021.
//

#ifndef BSW_SWITCH_BY_STRING_HH
#define BSW_SWITCH_BY_STRING_HH

// https://learnmoderncpp.com/2020/06/01/strings-as-switch-case-labels/

#include <bsw/strings/string_constant.hh>

constexpr inline auto switcher (const char* s) {
  unsigned long long hash{}, c{};
  for (auto p = s; *p; ++p, ++c) {
    hash += *p << c;
  }
  return hash;
}

constexpr inline auto operator "" _case (const char* s, size_t) {
  return switcher (s);
}

#endif //BSW_SWITCH_BY_STRING_HH
