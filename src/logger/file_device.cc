#include <iostream>
#include <fstream>

#include "bsw/logger/file_device.hh"
#include "bsw/logger/local_device.hh"
#include "bsw/logger/standard_layout.hh"
#include "bsw/logger/simple_printer.hh"
#include "bsw/logger/formatter.hh"
#include "bsw/logger/log_record_layout.hh"


namespace logger
{
    class file_c : public local_device
    {
    public:
        file_c(formatter_ptr_t formatter, predicate_ptr_t predicate, const bsw::filesystem::path& file);
    private:
        bool     _open_device() override;
        bool     _do_print_string(const std::string& data) override;
        void     _close_device() override;
    private:
        bsw::filesystem::path m_path;
        std::ofstream* m_ofs;
    };

    file_c::file_c(formatter_ptr_t formatter, predicate_ptr_t predicate, const bsw::filesystem::path& path)
        : local_device(formatter, predicate),
        m_path(path),
        m_ofs(nullptr)
    {
    }
    // -----------------------------------------------------------------
    bool file_c::_open_device()
    {
        m_ofs = new std::ofstream;

        m_ofs->open(m_path.u8string());

        if (*m_ofs)
        {
            return true;
        }
        return false;
    }
    // -----------------------------------------------------------------
    bool file_c::_do_print_string(const std::string& data)
    {
        if (!data.empty())
        {
            (*m_ofs) << data << std::endl;
        }
        m_ofs->flush();
        return true;
    }
    // -----------------------------------------------------------------
    void file_c::_close_device()
    {
        if (m_ofs)
        {
            m_ofs->flush();
            delete m_ofs;
            m_ofs = nullptr;
        }
    }
    // -----------------------------------------------------------------
    device_ptr_t file_device(const bsw::filesystem::path& path, predicate_ptr_t predicate)
    {
        log_record_layout_ptr_t layout_ptr(standard_layout());

        printer_ptr_t printer(new simple_printer);

        formatter_ptr_t fmatter(new formatter);


        fmatter->init(layout_ptr, printer);

        device_ptr_t console(new file_c(fmatter, predicate, path));
        return console;
    }
    // ----------------------------------------------------------------
    device_ptr_t file_device(const bsw::filesystem::path& path)
    {
        return file_device(path, nullptr);
    }
} // ns logger

