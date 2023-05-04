#include "bsw/logger/log_record_layout.hh"
#include "bsw/logger/layout_commands.hh"


namespace logger
{
    log_record_layout* standard_layout()
    {
        log_record_layout* layout = new log_record_layout;
        layout->append_time_value()->append_text(" ")->append_level()->
            append_text(" ")->append_text("@")->append_file_name()->append_text(":")->append_line_number()->
            append_space()->append_func_name()->append_text("> ")->append_text();
        return layout;
    }
} // ns logger

