#ifndef BSW_LOGGER_CONSOLE_HH
#define BSW_LOGGER_CONSOLE_HH

#include <bsw/logger/logger_fwd.hh>

namespace bsw::logger {
	BSW_EXPORT device_ptr_t console_device ();
	BSW_EXPORT device_ptr_t console_device (predicate_ptr_t p);
} // ns logger


#endif
