//
// Created by igor on 21/02/2021.
//

#ifndef BSW_WHEREAMI_HH
#define BSW_WHEREAMI_HH

#include <filesystem>
#include <bsw/export.h>

namespace bsw {
  BSW_EXPORT std::filesystem::path get_executable_path ();
  BSW_EXPORT std::filesystem::path get_module_path ();
}

#endif //BSW_WHEREAMI_HH
