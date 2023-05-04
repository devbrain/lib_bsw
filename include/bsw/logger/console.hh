#ifndef BSW_LOGGER_CONSOLE_HH
#define BSW_LOGGER_CONSOLE_HH

#include "bsw/logger/logger_fwd.hh"

namespace logger
{
    BSW_API device_ptr_t console_device();
    BSW_API device_ptr_t console_device(predicate_ptr_t p);
} // ns logger


#endif
