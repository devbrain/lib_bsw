#ifndef BSW_LOGGER_ROTATING_DEVICE_HH
#define BSW_LOGGER_ROTATING_DEVICE_HH

#include "bsw/logger/logger_fwd.hh"
#include "bsw/filesystem.hh"


namespace logger
{
    BSW_API device_ptr_t rotating_device(const bsw::filesystem::path& prefix, unsigned num, std::size_t max_lines);
    BSW_API device_ptr_t rotating_device(const bsw::filesystem::path& prefix, unsigned num, std::size_t max_lines, predicate_ptr_t predicate);
} // ns logger


#endif
