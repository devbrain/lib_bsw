#include "bsw/logger/predicate.hh"
#include "bsw/logger/record.hh"

namespace logger
{

    not_predicate_c::not_predicate_c(const predicate_ptr_t operand)
        : m_operand(operand)
    {
    }


    bool not_predicate_c::call(const record_ptr_t record) const
    {
        if (!m_operand)
        {
            return true;
        }
        return !m_operand->call(record);
    }

    or_predicate_c::or_predicate_c(const predicate_ptr_t operand1,
        const predicate_ptr_t operand2)
        : m_operand1(operand1),
        m_operand2(operand2)
    {
    }

    bool or_predicate_c::call(const record_ptr_t record) const
    {
        bool r1 = m_operand1 ? m_operand1->call(record) : true;
        if (r1)
        {
            return true;
        }
        return(m_operand2 ? m_operand2->call(record) : true);
    }

    and_predicate_c::and_predicate_c(const predicate_ptr_t operand1,
        const predicate_ptr_t operand2)
        : m_operand1(operand1),
        m_operand2(operand2)
    {
    }


    bool and_predicate_c::call(const record_ptr_t record) const
    {
        bool r1 = m_operand1 ? m_operand1->call(record) : true;
        if (!r1)
        {
            return false;
        }
        return(m_operand2 ? m_operand2->call(record) : true);
    }

    if_predicate_c::if_predicate_c(const predicate_ptr_t condition,
        const predicate_ptr_t yesBranch,
        const predicate_ptr_t noBranch)
        : m_condition(condition),
        m_yesBranch(yesBranch),
        m_noBranch(noBranch)
    {
    }



    bool if_predicate_c::call(const record_ptr_t record) const
    {
        if (!m_condition)
        {
            return true;
        }

        if (m_condition->call(record))
        {
            return m_yesBranch->call(record);
        }
        else
        {
            return m_noBranch->call(record);
        }

    }

    level_eq_c::level_eq_c(level_t level)
        : m_levelMin(level),
        m_levelMax(level),
        m_checkRange(false)
    {
    }

    level_eq_c::level_eq_c(level_t levelMin, level_t levelMax)
        : m_levelMin(levelMin),
        m_levelMax(levelMax),
        m_checkRange(true)
    {
    }

    bool level_eq_c::call(const record_ptr_t record) const
    {
        level_t level = record->level();
        if (m_checkRange)
        {
            return(m_levelMin <= level && level <= m_levelMax);
        }
        return (m_levelMin == level);
    }


    line_num_eq_c::line_num_eq_c(unsigned int line)
        : m_lineMin(line),
        m_lineMax(0),
        m_checkRange(false)
    {

    }

    line_num_eq_c::line_num_eq_c(unsigned int lineMin, unsigned int lineMax)
        : m_lineMin(lineMin),
        m_lineMax(lineMax),
        m_checkRange(true)
    {
    }

    bool line_num_eq_c::call(const record_ptr_t record) const
    {
        unsigned int line = record->get_locus().line();
        if (m_checkRange)
        {
            return(m_lineMin <= line && line <= m_lineMax);
        }
        return(line == m_lineMin);
    }


    file_name_eq_c::file_name_eq_c(const std::string& fileName)
        : m_fileName(fileName)
    {
    }

    bool file_name_eq_c::call(const record_ptr_t record) const
    {
        return(record->get_locus().source() == m_fileName);
    }


    module_name_predicate_c::module_name_predicate_c(const char* moduleName)
        : m_moduleName(moduleName)
    {
    }

    bool module_name_predicate_c::call(const record_ptr_t record) const
    {
        return (m_moduleName == record->get_locus().module());
    }


    tag_predicate_c::tag_predicate_c(const char* tag)
        : m_tag(tag)
    {
    }

    bool tag_predicate_c::call(const record_ptr_t record) const
    {
        return (m_tag == record->tag());
    }
} // ns logger


