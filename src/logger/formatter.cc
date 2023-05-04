#include "bsw/logger/formatter.hh"
#include "bsw/logger/log_record_layout.hh"
#include "bsw/logger/abstract_printer.hh"


namespace logger
{
    formatter::formatter()
    {
    }
    // ---------------------------------------------------------
    formatter::~formatter()
    {

    }
    // ---------------------------------------------------------
    void formatter::init(log_record_layout_ptr_t layout, printer_ptr_t printer)
    {
        m_layout = layout;
        m_printer = printer;
    }
    // ---------------------------------------------------------
    void formatter::start(std::ostream& stream)
    {
        m_printer->print_prologue(stream);
    }
    // ---------------------------------------------------------
    void formatter::done(std::ostream& stream)
    {
        m_printer->print_epilogue(stream);
    }
    // ---------------------------------------------------------
    void formatter::print(const record_ptr_t record, std::ostream& stream)
    {
        m_printer->before_log_record(stream, record);
        m_layout->apply(m_printer, stream, record);
        m_printer->after_log_record(stream, record);
    }
} // ns logger


