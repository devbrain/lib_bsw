#ifndef BSW_LOGGER_RECORD_HH
#define BSW_LOGGER_RECORD_HH

#include <string>
#include <vector>
#include <cstdint>
#include <chrono>

#include <bsw/logger/locus.hh>
#include <bsw/logger/level.hh>
#include <bsw/logger/thread_id.hh>
#include <bsw/export.h>

namespace bsw::logger {
	namespace priv {
		class multi_setter_c;
	}

	class BSW_EXPORT record {
		friend class priv::multi_setter_c;

	 public:
		record (level_t level, const locus& locus);

		[[nodiscard]] level_t level () const;
		[[nodiscard]] const locus& get_locus () const;
		[[nodiscard]] const std::string& message () const;

		[[nodiscard]] const std::chrono::system_clock::time_point& time () const;
		void time (const std::chrono::system_clock::time_point& t);

		[[nodiscard]] mt::thread_id_t thread_id () const;
		void thread_id (int tid);

		[[nodiscard]] int pid () const;
		void pid (int p);

		[[nodiscard]] bool is64 () const;

		[[nodiscard]] const std::string& tag () const;
		void tag (const std::string& x);

	 private:
		level_t m_level;
		locus m_locus;

#include <bsw/warn/push.hh>
#include <bsw/warn/dll_interface>

		std::string m_message;
		std::string m_tag;
		std::chrono::system_clock::time_point m_time;

#include <bsw/warn/pop.hh>

		int32_t m_thread_id;
		int32_t m_pid;
		bool m_is64;

	};
} // ns logger


#endif
