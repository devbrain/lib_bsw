#include <sstream>
#include <iostream>

#include "bsw/logger/local_device.hh"
#include "bsw/logger/formatter.hh"
#include "bsw/logger/record.hh"


namespace logger
{

    // =======================================================================
    local_device::local_device(formatter_ptr_t formatter, predicate_ptr_t predicate)
        : basic_device(predicate),
        m_formatter(formatter)
    {
    }

    // -----------------------------------------------------------------------
    bool local_device::_do_open()
    {
        if (_open_device())
        {
            if (m_formatter)
            {
                std::ostringstream ostream;
                m_formatter->start(ostream);
                _do_print_string(ostream.str());
            }
            return true;
        }
        return false;
    }
    // ------------------------------------------------------------------------
    void local_device::_do_close()
    {
        if (m_formatter)
        {
            std::ostringstream ostream;
            m_formatter->done(ostream);
            _do_print_string(ostream.str());
        }
        _close_device();
    }
    // -----------------------------------------------------------------------
    formatter_ptr_t local_device::_formatter() const
    {
        return m_formatter;
    }
    // ------------------------------------------------------------------------
    bool local_device::_do_print(record_ptr_t record)
    {
        if (!m_formatter)
        {
            return false;
        }
        std::ostringstream ostream;
        m_formatter->print(record, ostream);
        return _do_print_string(ostream.str());
    }
} // ns logger


