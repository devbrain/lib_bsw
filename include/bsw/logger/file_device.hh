#ifndef BSW_LOGGER_FILE_DEVICE_HH
#define BSW_LOGGER_FILE_DEVICE_HH

#include <filesystem>
#include <bsw/logger/logger_fwd.hh>

namespace bsw::logger
{
    BSW_EXPORT device_ptr_t file_device(const std::filesystem::path& path);
    BSW_EXPORT device_ptr_t file_device(const std::filesystem::path& path, predicate_ptr_t predicate);
} // ns logger


#endif
