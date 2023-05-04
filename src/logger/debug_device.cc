#include "bsw/predef.h"
#if defined(PREDEF_PLATFORM_WIN32)
#include <Windows.h>
#else
#include <syslog.h>
#endif
#include <sstream>

#include "bsw/logger/debug_device.hh"
#include "bsw/logger/local_device.hh"
#include "bsw/logger/formatter.hh"
#include "bsw/logger/standard_layout.hh"
#include "bsw/logger/simple_printer.hh"
#include "bsw/logger/log_record_layout.hh"


namespace logger
{
    class debug_console_c : public local_device
    {
    public:
        debug_console_c(formatter_ptr_t formatter, predicate_ptr_t predicate);
    private:
        bool     _open_device() override;
        bool     _do_print_string(const std::string& data) override;
        void     _close_device() override;
        bool     _do_print(record_ptr_t record) override;
    };

    debug_console_c::debug_console_c(formatter_ptr_t formatter, predicate_ptr_t predicate)
        : local_device(formatter, predicate)
    {

    }
    // -----------------------------------------------------------------
    bool debug_console_c::_do_print(record_ptr_t record)
    {
        std::ostringstream ostream;
        _formatter()->print(record, ostream);
        ostream << "\r\n";
        _do_print_string(ostream.str());

        return true;
    }
    // -----------------------------------------------------------------
    bool debug_console_c::_open_device()
    {
#if defined(PREDEF_PLATFORM_WIN32)
        return true;
#else
        openlog("BINADOX", LOG_PID | LOG_CONS, LOG_USER);
        return true;
#endif
    }
    // -----------------------------------------------------------------
    bool debug_console_c::_do_print_string(const std::string& data)
    {
        if (!data.empty())
        {
#if defined(PREDEF_PLATFORM_WIN32)

            OutputDebugStringA(data.c_str());
#else
            syslog(LOG_NOTICE, "%s", data.c_str());
#endif
        }
        return true;
    }
    // -----------------------------------------------------------------
    void debug_console_c::_close_device()
    {
    }
    // ------------------------------------------------------------------
    device_ptr_t debug_console_device(predicate_ptr_t predicate)
    {
        log_record_layout_ptr_t layout(standard_layout());

        printer_ptr_t printer(new simple_printer);

        formatter_ptr_t fmatter(new formatter);

        fmatter->init(layout, printer);

        device_ptr_t debug_console(new debug_console_c(fmatter, predicate));
        return debug_console;
    }
    // -----------------------------------------------------------------
    device_ptr_t debug_console_device()
    {
        predicate_ptr_t null_predicate = nullptr;
        return logger::debug_console_device(null_predicate);
    }
} // ns logger

