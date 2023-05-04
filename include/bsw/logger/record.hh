#ifndef BSW_LOGGER_RECORD_HH
#define BSW_LOGGER_RECORD_HH

#include <string>
#include <vector>
#include <cstdint>
#include <chrono>

#include "bsw/logger/locus.hh"
#include "bsw/logger/level.hh"
#include "bsw/logger/thread_id.hh"
#include "bsw/bsw_api.h"

namespace logger
{
    namespace priv
    {
        class multi_setter_c;
    }

    class BSW_API record
    {
        friend class priv::multi_setter_c;

    public:
        record(level_t level, const locus& locus);

        level_t level() const;
        const locus& get_locus() const;
        const std::string& message() const;

        const std::chrono::system_clock::time_point& time() const;
        void time(const std::chrono::system_clock::time_point& t);

        mt::thread_id_t thread_id() const;
        void thread_id(int tid);

        int pid() const;
        void pid(int p);

        bool is64() const;

        const std::string& tag() const;
        void tag(const std::string& x);


    private:
        level_t         m_level;
        locus           m_locus;

#include <bsw/warn/push.hh>
#include <bsw/warn/dll_interface>

        std::string      m_message;
        std::string      m_tag;
        std::chrono::system_clock::time_point m_time;

#include <bsw/warn/pop.hh>

        int32_t          m_thread_id;
        int32_t          m_pid;
        bool             m_is64;

    };
} // ns logger


#endif
