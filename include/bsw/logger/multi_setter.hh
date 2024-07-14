#ifndef LOGGER_MULTI_SETTER_HH
#define LOGGER_MULTI_SETTER_HH

#include <sstream>
#include <chrono>

#include <filesystem>
#include <bsw/logger/record.hh>

namespace bsw::logger::priv {
	inline
	std::ostream& operator<< (std::ostream& os, const std::filesystem::path& s) {
		os << s.u8string ();
		return os;
	}

	inline
	std::ostream& operator<< (std::ostream& os, const std::chrono::hours& s) {
		os << s.count () << "hr.";
		return os;
	}

	inline
	std::ostream& operator<< (std::ostream& os, std::chrono::hours&& s) {
		os << s.count () << "hr.";
		return os;
	}

	inline
	std::ostream& operator<< (std::ostream& os, const std::chrono::seconds& s) {
		os << s.count () << "sec.";
		return os;
	}

	inline
	std::ostream& operator<< (std::ostream& os, std::chrono::seconds&& s) {
		os << s.count () << "sec.";
		return os;
	}

	inline
	std::ostream& operator<< (std::ostream& os, const std::chrono::milliseconds& s) {
		os << s.count () << "ms.";
		return os;
	}

	inline
	std::ostream& operator<< (std::ostream& os, std::chrono::milliseconds&& s) {
		os << s.count () << "ms.";
		return os;
	}

	/////////////////////////////////////////////////////////////////////////
	// implementation

	// base case, terminating recursion
	template <typename Arg0>
	inline void print_impl (std::ostream& os, Arg0&& arg0) {
		os << std::forward<Arg0> (arg0);
	}

	template <typename Arg0, typename... Args>
	inline void print_impl (std::ostream& os, Arg0&& arg0, Args&& ... rest) {
		// print the first argument.
		os << std::forward<Arg0> (arg0);
		// recursion for rest arguments
		print_impl (os, std::forward<Args> (rest)...); // this resolves to either myself or print(arg0)
	}

	class multi_printer {
	 public:
		explicit multi_printer (record& record)
			: m_record (record) {
		}

		template <typename... Args>
		void print (Args&& ... rest) {
			std::ostringstream os;
			print_impl (os, std::forward<Args> (rest)...);
			m_record.m_message = os.str ();
		}

	 private:
		record& m_record;
	};
} // ns priv
// ns logger



#endif
