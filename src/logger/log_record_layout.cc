#include "bsw/logger/log_record_layout.hh"
#include "bsw/logger/record.hh"
#include "bsw/logger/abstract_printer.hh"
#include "bsw/logger/layout_commands.hh"


namespace logger
{
    log_record_layout::~log_record_layout()
    {
    }

    namespace
    {
        class severity_command_c : public abstract_layout_command
        {
        public:
            virtual void call(printer_ptr_t printer,
                std::ostream& stream,
                const record_ptr_t record) const;
        };
        // -----------------------------------------------------------
        void severity_command_c::call(printer_ptr_t printer,
            std::ostream& stream,
            const record_ptr_t record) const
        {
            printer->print_level(stream, record);
        }
    } // unnamed ns
    // ====================================================================
    log_record_layout* log_record_layout::append_level()
    {
        this->_add_command(layout_command_ptr_t(new severity_command_c));
        return this;
    }
} // ns logger

