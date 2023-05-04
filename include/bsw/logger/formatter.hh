#ifndef BSW_LOGGER_FORMATTER_HH
#define BSW_LOGGER_FORMATTER_HH

#include <iosfwd>

#include "bsw/logger/logger_fwd.hh"



namespace logger
{
    class formatter
    {
    public:
        formatter();
        ~formatter();

        void init(log_record_layout_ptr_t layout, printer_ptr_t printer);

        void start(std::ostream& stream);
        void done(std::ostream& stream);

        void print(const record_ptr_t record, std::ostream& stream);
    private:
#include <bsw/warn/push.hh>
#include <bsw/warn/dll_interface>
        log_record_layout_ptr_t m_layout;
		printer_ptr_t           m_printer;
#include <bsw/warn/pop.hh>
        

    };
} // ns logger


#endif
