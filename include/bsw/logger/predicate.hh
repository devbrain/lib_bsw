#ifndef BSW_LOGGER_PREDICATE_HH
#define BSW_LOGGER_PREDICATE_HH

#include <string>
#include "bsw/logger/level.hh"
#include "bsw/logger/logger_fwd.hh"



namespace logger
{
    class predicate
    {
    public:
        predicate() = default;
        virtual ~predicate() =default;

        virtual bool call(const record_ptr_t record) const = 0;
    };

    class  not_predicate_c : public predicate
    {
    public:
        explicit not_predicate_c(const predicate_ptr_t operand);

        virtual bool call(const record_ptr_t record) const;
    private:
        const predicate_ptr_t m_operand;
    };

    class  or_predicate_c : public predicate
    {
    public:
        or_predicate_c(const predicate_ptr_t operand1, const predicate_ptr_t operand2);

        virtual bool call(const record_ptr_t record) const;
    private:
        const predicate_ptr_t m_operand1;
        const predicate_ptr_t m_operand2;

    };

    class  and_predicate_c : public predicate
    {
    public:
        and_predicate_c(const predicate_ptr_t operand1, const predicate_ptr_t operand2);

        virtual bool call(const record_ptr_t record) const;
    private:

        const predicate_ptr_t m_operand1;
        const predicate_ptr_t m_operand2;

    };

    class  if_predicate_c : public predicate
    {
    public:
        if_predicate_c(const predicate_ptr_t condition,
            const predicate_ptr_t yesBranch,
            const predicate_ptr_t noBranch);

        virtual bool call(const record_ptr_t record) const;
    private:

        const predicate_ptr_t m_condition;
        const predicate_ptr_t m_yesBranch;
        const predicate_ptr_t m_noBranch;

    };

    class  level_eq_c : public predicate
    {
    public:
        explicit level_eq_c(level_t level);
        level_eq_c(level_t levelMin, level_t levelMax);

        virtual bool call(const record_ptr_t record) const;
    private:
        const level_t m_levelMin;
        const level_t m_levelMax;
        bool          m_checkRange;
    };

    class  line_num_eq_c : public predicate
    {
    public:
        explicit line_num_eq_c(unsigned int line);
        line_num_eq_c(unsigned int lineMin, unsigned int lineMax);
        virtual bool call(const record_ptr_t record) const;
    private:
        const unsigned int m_lineMin;
        const unsigned int m_lineMax;
        bool               m_checkRange;
    };

    class  file_name_eq_c : public predicate
    {
    public:
        explicit file_name_eq_c(const std::string& fileName);
        virtual bool call(const record_ptr_t record) const;
    private:

        const std::string m_fileName;

    };

    class  module_name_predicate_c : public predicate
    {
    public:
        explicit module_name_predicate_c(const char* moduleName);
        virtual bool call(const record_ptr_t record) const;
    private:

        const std::string m_moduleName;

    };

    class  tag_predicate_c : public predicate
    {
    public:
        explicit tag_predicate_c(const char* tag);
        virtual bool call(const record_ptr_t record) const;
    private:
        const std::string m_tag;
    };
} // ns logger




#endif
