#ifndef BSW_LOGGER_LAYOUT_COMMANDS_HH
#define BSW_LOGGER_LAYOUT_COMMANDS_HH

#include <string>

#include "bsw/logger/logger_fwd.hh"


namespace logger
{
    class abstract_layout_command
    {
    public:
        virtual ~abstract_layout_command();
        virtual void call(printer_ptr_t printer, std::ostream& stream, const record_ptr_t record) const = 0;
    };

    class space_command : public abstract_layout_command
    {
    public:
        explicit space_command(unsigned int count);
        virtual void call(printer_ptr_t printer, std::ostream& stream, const record_ptr_t record) const;
    private:
        unsigned int m_count;
    };

    class file_name_command : public abstract_layout_command
    {
    public:
        virtual void call(printer_ptr_t printer, std::ostream& stream, const record_ptr_t record) const;
    };

    class func_name_command : public abstract_layout_command
    {
    public:
        virtual void call(printer_ptr_t printer, std::ostream& stream, const record_ptr_t record) const;
    };

    class line_number_command : public abstract_layout_command
    {
    public:
        virtual void call(printer_ptr_t printer, std::ostream& stream, const record_ptr_t record) const;
    };

    class thread_id_command : public abstract_layout_command
    {
    public:
        virtual void call(printer_ptr_t printer, std::ostream& stream, const record_ptr_t record) const;
    };

    class time_value_command : public abstract_layout_command
    {
    public:
        virtual void call(printer_ptr_t printer, std::ostream& stream, const record_ptr_t record) const;
    };

    class module_name_command : public abstract_layout_command
    {
    public:
        virtual void call(printer_ptr_t printer, std::ostream& stream, const record_ptr_t record) const;
    };

    class text_command : public abstract_layout_command
    {
    public:
        virtual void call(printer_ptr_t printer, std::ostream& stream, const record_ptr_t record) const;
    };

    class free_text_command : public abstract_layout_command
    {
    public:
        explicit free_text_command(const std::string& txt);
        virtual void call(printer_ptr_t printer, std::ostream& stream, const record_ptr_t record) const;
    private:
        std::string m_txt;
    };
} // ns logger


#endif

