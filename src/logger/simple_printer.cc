#include <cstring>
#include <ostream>
#include <ctime>
#include <iomanip> 

#include "bsw/logger/simple_printer.hh"
#include "bsw/logger/record.hh"

#if !defined(__clang__)
#if defined(__GNUC__) && (__GNUC__ < 5)
#define LOGGER_NO_PUT_TIME
#endif 
#endif

#if defined(_MSC_VER)
#define snprintf _snprintf
#endif


namespace logger
{
    void simple_printer::print_function_name(std::ostream& stream, const record_ptr_t record)
    {
        auto fnc = record->get_locus().function();
        auto ptr = strstr(fnc, "<lambda_");
        if (!ptr)
        {
            stream << fnc;
        }
        else
        {
            auto n = ptr - fnc;
            stream.write(fnc, n);
            stream << "lambda";
        }
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::print_space(std::ostream& stream, unsigned int count)
    {
        for (unsigned int i = 0; i < count; i++)
        {
            stream << ' ';
        }
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::print_level(std::ostream& stream, const record_ptr_t record)
    {
        stream << record->level();
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::print_file_name(std::ostream& stream, const record_ptr_t record)
    {
        const std::string& src = record->get_locus().source();
        std::size_t p = src.find("src");
        if (p == std::string::npos)
        {
            stream << src;
        }
        else
        {
            stream << src.c_str() + p;
        }
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::print_line_number(std::ostream& stream, const record_ptr_t record)
    {
        stream << record->get_locus().line();
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::print_thread_id(std::ostream& stream, const record_ptr_t record)
    {
        const auto tid = record->thread_id();
        stream << tid;
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::print_time_value(std::ostream& stream, const record_ptr_t record)
    {
        auto in_time_t = std::chrono::system_clock::to_time_t(record->time());
#if !defined(LOGGER_NO_PUT_TIME)
        stream << std::put_time(std::localtime(&in_time_t), "%m-%d %H:%M:%S");
#else
        struct tm tms;
        if (localtime_r(&in_time_t, &tms))
        {
            char tmbuf[64], buf[64];
            strftime(tmbuf, sizeof tmbuf, "%m-%d %H:%M:%S", &tms);
            /*
      #if defined(__APPLE__)
            #define FMT "%s.%06d"
      #else
            #define FMT "%s.%06ld"
      #endif
                snprintf (buf, sizeof buf, FMT, tmbuf, tv.tv_usec);
            */
            stream << tmbuf;
        }
#endif
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::print_module_name(std::ostream& stream, const record_ptr_t record)
    {
        stream << record->get_locus().module();
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::print_args(std::ostream& stream, const record_ptr_t record)
    {
        stream << record->message();
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::print_free_text(std::ostream& stream, const std::string& text)
    {
        stream << text;
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::print_prologue(std::ostream& /*stream*/)
    {
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::print_epilogue(std::ostream& /*stream*/)
    {
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::before_log_record(std::ostream& /*stream*/, const record_ptr_t /*record*/)
    {
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::after_log_record(std::ostream& /*stream*/, const record_ptr_t /*record*/)
    {
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::before_print_args(std::ostream& /*stream*/, const record_ptr_t /*record*/)
    {
    }
    // -----------------------------------------------------------------------------------------
    void simple_printer::after_print_args(std::ostream& /*stream*/, const record_ptr_t /*record*/)
    {
    }
} // ns logger

