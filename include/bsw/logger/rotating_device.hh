#ifndef BSW_LOGGER_ROTATING_DEVICE_HH
#define BSW_LOGGER_ROTATING_DEVICE_HH

#include <bsw/logger/logger_fwd.hh>
#include <filesystem>

namespace bsw::logger {
	BSW_EXPORT device_ptr_t rotating_device (const std::filesystem::path& prefix, unsigned num, std::size_t max_lines);
	BSW_EXPORT device_ptr_t
	rotating_device (const std::filesystem::path& prefix, unsigned num, std::size_t max_lines, predicate_ptr_t predicate);
} // ns logger


#endif
