//
// Created by igor on 21/02/2021.
//

#ifndef BSW_WHEREAMI_HH
#define BSW_WHEREAMI_HH

#include <filesystem>

namespace bsw {
  std::filesystem::path get_executable_path ();
  std::filesystem::path get_module_path ();
}

#endif //BSW_WHEREAMI_HH
