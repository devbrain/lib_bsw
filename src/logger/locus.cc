#include "bsw/logger/locus.hh"


namespace logger
{

    locus::locus(const char* module_name,
        const char* source,
        const char* function,
        unsigned int line)
        : m_module_name(const_cast <char*>(module_name)),
        m_src_file(const_cast <char*>(source)),
        m_function(const_cast <char*>(function)),
        m_line(line),
        m_is_owner(false)
    {
    }
    // ----------------------------------------------------------
    locus::~locus()
    {
        if (!m_is_owner)
        {
            return;
        }
        delete[] m_src_file;
        delete[] m_function;
        delete[] m_module_name;
    }
    // ----------------------------------------------------------
    const char* locus::module() const
    {
        return m_module_name;
    }
    // ----------------------------------------------------------
    const char* locus::source() const
    {
        return m_src_file;
    }
    // ----------------------------------------------------------
    const char* locus::function() const
    {
        return m_function;
    }
    // ----------------------------------------------------------
    unsigned int locus::line() const
    {
        return m_line;
    }
} // ns logger


