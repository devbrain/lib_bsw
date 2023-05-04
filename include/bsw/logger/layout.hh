#ifndef BSW_LOGGER_LAYOUT_HH
#define BSW_LOGGER_LAYOUT_HH

#include <string>
#include <iosfwd>

#include <list>

#include "bsw/logger/logger_fwd.hh"
#include "bsw/logger/layout_commands.hh"


namespace logger
{

    class BSW_API applyer
    {
    public:
        virtual ~applyer();
        void apply(printer_ptr_t printer, std::ostream& stream, const record_ptr_t record) const;
    protected:
        void _add_command(layout_command_ptr_t cmd);
    private:
        typedef std::list <layout_command_ptr_t> commands_list_t;
    private:
#include <bsw/warn/push.hh>
#include <bsw/warn/dll_interface>
        commands_list_t m_commands;
#include <bsw/warn/pop.hh>

    };

    template <class Derived>
    class basic_layout_c : public applyer
    {
    public:

        Derived * append_space(unsigned int count = 1);
        Derived* append_file_name(void);
        Derived* append_func_name(void);
        Derived* append_line_number(void);
        Derived* append_thread_id(void);
        Derived* append_time_value(void);
        Derived* append_module_name(void);
        Derived* append_text(void);
        Derived* append_text(const std::string& text);
    };

} // ns logger


// ====================================================================
// Implementation
// ====================================================================

namespace logger
{

    template <class Derived>
    Derived* basic_layout_c<Derived>::append_space(unsigned int count)
    {
        this->_add_command(layout_command_ptr_t(new space_command(count)));
        return dynamic_cast<Derived*>(this);
    }

    template <class Derived>
    Derived* basic_layout_c<Derived>::append_file_name(void)
    {
        this->_add_command(layout_command_ptr_t(new file_name_command));
        return dynamic_cast<Derived*>(this);
    }

    template <class Derived>
    Derived* basic_layout_c<Derived>::append_func_name(void)
    {
        this->_add_command(layout_command_ptr_t(new func_name_command));
        return dynamic_cast<Derived*>(this);
    }

    template <class Derived>
    Derived* basic_layout_c<Derived>::append_line_number(void)
    {
        this->_add_command(layout_command_ptr_t(new line_number_command));
        return dynamic_cast<Derived*>(this);
    }

    template <class Derived>
    Derived* basic_layout_c<Derived>::append_thread_id(void)
    {
        this->_add_command(layout_command_ptr_t(new thread_id_command));
        return dynamic_cast<Derived*>(this);
    }

    template <class Derived>
    Derived* basic_layout_c<Derived>::append_time_value(void)
    {
        this->_add_command(layout_command_ptr_t(new time_value_command));
        return dynamic_cast<Derived*>(this);
    }

    template <class Derived>
    Derived* basic_layout_c<Derived>::append_module_name(void)
    {
        this->_add_command(layout_command_ptr_t(new module_name_command));
        return dynamic_cast<Derived*>(this);
    }

    template <class Derived>
    Derived* basic_layout_c<Derived>::append_text(void)
    {
        this->_add_command(layout_command_ptr_t(new text_command));
        return dynamic_cast<Derived*>(this);
    }

    template <class Derived>
    Derived* basic_layout_c<Derived>::append_text(const std::string& text)
    {
        this->_add_command(layout_command_ptr_t(new free_text_command(text)));
        return dynamic_cast<Derived*>(this);
    }
} // ns logger

#endif


