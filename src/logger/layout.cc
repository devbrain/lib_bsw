#include "bsw/logger/layout.hh"
#include "bsw/logger/layout_commands.hh"


namespace logger
{
    // ----------------------------------------------------------------------------
    applyer::~applyer()
    {

    }
    // ----------------------------------------------------------------------------
    void applyer::apply(printer_ptr_t printer, std::ostream& stream, const record_ptr_t record) const
    {

        for (auto cmd : m_commands)
        {
            cmd->call(printer, stream, record);
        }
    }
    // ----------------------------------------------------------------------------
    void applyer::_add_command(layout_command_ptr_t cmd)
    {
        m_commands.push_back(cmd);
    }

} // ns logger


