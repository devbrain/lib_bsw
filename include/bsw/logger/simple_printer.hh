#ifndef BSW_LOGGER_SIMPLE_PRINTER_HH
#define BSW_LOGGER_SIMPLE_PRINTER_HH

#include "bsw/logger/abstract_printer.hh"


namespace logger
{
    class  simple_printer : public abstract_printer
    {
    public:
        void print_function_name(std::ostream& stream, const record_ptr_t record);
        void print_space(std::ostream& stream, unsigned int count);
        void print_level(std::ostream& stream, const record_ptr_t record);
        void print_file_name(std::ostream& stream, const record_ptr_t record);
        void print_line_number(std::ostream& stream, const record_ptr_t record);
        void print_thread_id(std::ostream& stream, const record_ptr_t record);
        void print_time_value(std::ostream& stream, const record_ptr_t record);
        void print_module_name(std::ostream& stream, const record_ptr_t record);
        void print_args(std::ostream& stream, const record_ptr_t record);
        void print_free_text(std::ostream& stream, const std::string& text);


        void print_prologue(std::ostream& stream);
        void print_epilogue(std::ostream& stream);
        void before_log_record(std::ostream& stream, const record_ptr_t record);
        void after_log_record(std::ostream& stream, const record_ptr_t record);
        void before_print_args(std::ostream& stream, const record_ptr_t record);
        void after_print_args(std::ostream& stream, const record_ptr_t record);
    };
} // ns logger


#endif
