#ifndef BSW_LOGGER_LOCUS_HH
#define BSW_LOGGER_LOCUS_HH

#include "bsw/bsw_api.h"

namespace logger
{
    class BSW_API locus
    {
    public:
        locus(const char* module_name,
            const char* source,
            const char* function,
            unsigned int line);
        locus(const locus&) = default;
        ~locus();

        const char* module() const;
        const char* source() const;
        const char* function() const;
        unsigned int line() const;


    private:
        char* m_module_name;
        char* m_src_file;
        char* m_function;
        unsigned int m_line;
        bool               m_is_owner;
    };
} // ns logger



#endif
