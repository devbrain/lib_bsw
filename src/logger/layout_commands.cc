#include "bsw/logger/layout_commands.hh"
#include "bsw/logger/abstract_printer.hh"

namespace logger
{

    abstract_layout_command::~abstract_layout_command()
    {

    }

    space_command::space_command(unsigned int count)
        : m_count(count)
    {
    }

    void space_command::call(printer_ptr_t printer,
        std::ostream& stream,
        const record_ptr_t /*record*/) const
    {
        printer->print_space(stream, m_count);
    }

    // FileNameCommand

    void file_name_command::call(printer_ptr_t printer,
        std::ostream& stream,
        const record_ptr_t record) const
    {
        printer->print_file_name(stream, record);
    }

    // FuncNameCommand

    void func_name_command::call(printer_ptr_t printer,
        std::ostream& stream,
        const record_ptr_t record) const
    {
        printer->print_function_name(stream, record);
    }

    // LineNumberCommand

    void line_number_command::call(printer_ptr_t printer,
        std::ostream& stream,
        const record_ptr_t record) const
    {
        printer->print_line_number(stream, record);
    }

    // TreadIDCommand

    void thread_id_command::call(printer_ptr_t printer,
        std::ostream& stream,
        const record_ptr_t record) const
    {
        printer->print_thread_id(stream, record);
    }

    // TimeValueCommand

    void time_value_command::call(printer_ptr_t printer,
        std::ostream& stream,
        const record_ptr_t record) const
    {
        printer->print_time_value(stream, record);
    }

    // ModuleNameCommand

    void module_name_command::call(printer_ptr_t printer,
        std::ostream& stream,
        const record_ptr_t record) const
    {
        printer->print_module_name(stream, record);
    }

    // TextCommand

    void text_command::call(printer_ptr_t printer,
        std::ostream& stream,
        const record_ptr_t record) const
    {
        printer->before_print_args(stream, record);
        printer->print_args(stream, record);
        printer->after_print_args(stream, record);
    }

    // FreeTextCommand
    free_text_command::free_text_command(const std::string& txt)
        : m_txt(txt)
    {
    }

    void free_text_command::call(printer_ptr_t printer,
        std::ostream& stream,
        const record_ptr_t /*record*/) const
    {
        printer->print_free_text(stream, m_txt);
    }


} // ns logger


