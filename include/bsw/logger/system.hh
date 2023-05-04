#ifndef BSW_LOGGER_SYSTEM_HH
#define BSW_LOGGER_SYSTEM_HH

#include <list>

#include "bsw/logger/logger_fwd.hh"
#include "bsw/bsw_api.h"

namespace logger
{

    void BSW_API add_device(device_ptr_t device);
    void BSW_API post(record_ptr_t record);
    void BSW_API shutdown();

    class auto_logger
    {
    public:
        ~auto_logger()
        {
            shutdown();
        }
    };

} // ns logger



#endif
