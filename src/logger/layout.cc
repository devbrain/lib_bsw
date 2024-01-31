#include <bsw/logger/layout.hh>
#include <bsw/logger/layout_commands.hh>


namespace bsw::logger
{
    // ----------------------------------------------------------------------------
    applyer::~applyer()
    = default;
    // ----------------------------------------------------------------------------
    void applyer::apply(const printer_ptr_t& printer, std::ostream& stream, const record_ptr_t& record) const
    {

        for (const auto& cmd : m_commands)
        {
            cmd->call(printer, stream, record);
        }
    }
    // ----------------------------------------------------------------------------
    void applyer::_add_command(const layout_command_ptr_t& cmd)
    {
        m_commands.push_back(cmd);
    }

} // ns logger


