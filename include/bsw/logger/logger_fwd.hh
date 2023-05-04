#ifndef BSW_LOGGER_FWD_HH
#define BSW_LOGGER_FWD_HH

#include <memory>
#include "bsw/bsw_api.h"

namespace logger
{
    class  record;
    typedef std::shared_ptr <record> record_ptr_t;

    class BSW_API predicate;
    typedef std::shared_ptr <predicate> predicate_ptr_t;

    class BSW_API formatter;
    typedef std::shared_ptr <formatter> formatter_ptr_t;

    class BSW_API abstract_printer;
    typedef std::shared_ptr<abstract_printer> printer_ptr_t;

    class BSW_API log_record_layout;
    typedef std::shared_ptr <log_record_layout> log_record_layout_ptr_t;

    class BSW_API formatter;
    typedef std::shared_ptr <formatter> formatter_ptr_t;

    class BSW_API abstract_layout_command;
    typedef std::shared_ptr <abstract_layout_command> layout_command_ptr_t;

    class BSW_API basic_device;
    typedef std::shared_ptr<basic_device> device_ptr_t;
} // ns logger


#endif
