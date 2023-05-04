#ifndef BSW_LOGGER_CONSOLE_PRINTER_HH
#define BSW_LOGGER_CONSOLE_PRINTER_HH

#include "bsw/logger/simple_printer.hh"


namespace logger
{
    class console_printer : public simple_printer
    {
    public:
        console_printer();
        void print_prologue(std::ostream& stream);
        void print_epilogue(std::ostream& stream);
        void before_log_record(std::ostream& stream, const record_ptr_t record);
        void after_log_record(std::ostream& stream, const record_ptr_t record);
        void before_print_args(std::ostream& stream, const record_ptr_t record);
        void after_print_args(std::ostream& stream, const record_ptr_t record);
    };
} // ns logger


#endif
