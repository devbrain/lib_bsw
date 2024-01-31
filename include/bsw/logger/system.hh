#ifndef BSW_LOGGER_SYSTEM_HH
#define BSW_LOGGER_SYSTEM_HH

#include <list>

#include <bsw/logger/logger_fwd.hh>
#include <bsw/export.h>

namespace bsw::logger
{

    void BSW_EXPORT add_device(const device_ptr_t& device);
    void BSW_EXPORT post(const record_ptr_t& record);
    void BSW_EXPORT shutdown();

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
