#ifndef BSW_LOGGER_DEBUG_DEVICE_HH
#define BSW_LOGGER_DEBUG_DEVICE_HH

#include "bsw/logger/logger_fwd.hh"


namespace logger
{
    BSW_API device_ptr_t debug_console_device();
    BSW_API device_ptr_t debug_console_device(predicate_ptr_t p);
} // ns logger


#endif
