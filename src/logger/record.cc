#include <bsw/logger/record.hh>
#include <bsw/bsw_config.h>

namespace bsw::logger {
	record::record (level_t level, const locus& locus)
		: m_level (level),
		  m_locus (locus),
		  m_thread_id (static_cast<int32_t>(mt::current_thread_id ())),
		  m_pid (mt::get_current_pid ()),
#if BSW_IS_64_BIT
		m_is64(true)
#else
		  m_is64 (false)
#endif
	{
		m_time = std::chrono::system_clock::now ();
	}

	// ---------------------------------------------------------------
	level_t record::level () const {
		return m_level;
	}

	// ---------------------------------------------------------------
	const locus& record::get_locus () const {
		return m_locus;
	}

	// ---------------------------------------------------------------
	const std::string& record::message () const {
		return m_message;
	}

	// ---------------------------------------------------------------
	const std::chrono::system_clock::time_point& record::time () const {
		return m_time;
	}

	// ---------------------------------------------------------------
	void record::time (const std::chrono::system_clock::time_point& t) {
		m_time = t;
	}

	// ---------------------------------------------------------------
	mt::thread_id_t record::thread_id () const {
		return m_thread_id;
	}

	// ---------------------------------------------------------------
	void record::thread_id (int tid) {
		m_thread_id = tid;
	}

	// ---------------------------------------------------------------
	int record::pid () const {
		return m_pid;
	}

	// ---------------------------------------------------------------
	void record::pid (int p) {
		m_pid = p;
	}

	// ---------------------------------------------------------------
	bool record::is64 () const {
		return m_is64;
	}

	// ---------------------------------------------------------------
	const std::string& record::tag () const {
		return m_tag;
	}

	// ---------------------------------------------------------------
	void record::tag (const std::string& x) {
		m_tag = x;
	}
} // ns logger


