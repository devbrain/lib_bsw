#ifndef BSW_LOGGER_FILE_DEVICE_HH
#define BSW_LOGGER_FILE_DEVICE_HH

#include "bsw/logger/logger_fwd.hh"
#include "bsw/filesystem.hh"


namespace logger
{
    BSW_API device_ptr_t file_device(const bsw::filesystem::path& path);
    BSW_API device_ptr_t file_device(const bsw::filesystem::path& path, predicate_ptr_t predicate);
} // ns logger


#endif
