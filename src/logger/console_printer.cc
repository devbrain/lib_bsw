#include "bsw/logger/console_printer.hh"
#include "bsw/logger/record.hh"



namespace logger
{
    console_printer::console_printer()
    {

    }
    // ----------------------------------------------------------------------------------------
    void console_printer::print_prologue(std::ostream& /*stream*/)
    {
    }
    // ----------------------------------------------------------------------------------------
    void console_printer::print_epilogue(std::ostream& /*stream*/)
    {
    }
    // ----------------------------------------------------------------------------------------
    void console_printer::before_log_record(std::ostream& /*stream*/, const record_ptr_t /*record*/)
    {

    }
    // ----------------------------------------------------------------------------------------
    void console_printer::after_log_record(std::ostream& /*stream*/, const record_ptr_t /*record*/)
    {

    }
    // ----------------------------------------------------------------------------------------
    void console_printer::before_print_args(std::ostream& /*stream*/, const record_ptr_t /*record*/)
    {

    }
    // ----------------------------------------------------------------------------------------
    void console_printer::after_print_args(std::ostream& /*stream*/, const record_ptr_t /*record*/)
    {

    }
} // ns logger

