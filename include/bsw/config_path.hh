//
// Created by igor on 9/8/24.
//

#ifndef BSW_CONFIG_PATH_HH
#define BSW_CONFIG_PATH_HH

#include <filesystem>
#include <bsw/export.h>

namespace bsw {
    BSW_EXPORT std::filesystem::path get_config_path();
}

#endif
