#ifndef BSW_LOGGER_LEVEL_HH
#define BSW_LOGGER_LEVEL_HH

#include <iosfwd>


namespace logger
{
    enum level_t
    {
        eDEBUG,
        eINFO,
        eNOTICE,
        eWARNING,
        eERROR,
        eFATAL
    };

    std::ostream& operator << (std::ostream& os, level_t level);

} // ns logger



#endif
