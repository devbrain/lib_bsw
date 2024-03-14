//
// Created by igor on 31/05/2020.
//

#ifndef BSW_EXCEPTION_HH
#define BSW_EXCEPTION_HH

#include <bsw/export.h>
#include <sstream>
#include <string>
#include <stdexcept>
#include <memory>
#include <bsw/macros.hh>

namespace bsw {

	class BSW_EXPORT exception : public std::exception {
	 public:
		static bool use_function_signature (bool v);
	 public:
		template <typename ... Args>
		exception (const char* function, const char* source, int line, Args&& ... args);

		template <typename ... Args>
		exception (exception cause, const char* function, const char* source, int line, Args&& ... args);

		exception (exception&&) = default;

		exception(exception&) = default;

		~exception () override = default;

		const char* what () const noexcept override;

		[[nodiscard]] std::string function () const noexcept {
			return m_function;
		}

		[[nodiscard]] std::string source () const noexcept {
			return m_source;
		}

		[[nodiscard]] int line () const noexcept {
			return m_line;
		}

		[[nodiscard]] std::string text () const noexcept {
			return m_text;
		}

		[[nodiscard]] const exception* cause () const noexcept {
			if (m_chain) {
				return m_chain.get ();
			}
			return nullptr;
		}

	 protected:
		enum class dummy_t {
			CUSTOM
		};

		template <typename ... Args>
		exception (dummy_t custom, std::string prefix, const char* function, const char* source, int line,
				   Args&& ... args);

	 private:
		template <typename ... Args>
		static std::string
		_create (bool use_locus, const char* function, const char* source, int line, Args&& ... args);

		static const char* _transform_source (const char* source);
	 private:
		std::string m_function;
		std::string m_source;
		int m_line;
		std::string m_text;

		std::shared_ptr<exception> m_chain;
		mutable std::string m_what;

		static bool s_use_function_sig;
	};
} // ns
// ==========================================================================
// Implementation
// ==========================================================================
namespace bsw {
	template <typename ... Args>
	inline
	exception::exception (const char* function, const char* source, int line, Args&& ... args)
		: m_function (function),
		  m_source (source),
		  m_line (line),
		  m_text (_create (false, function, source, line, std::forward<Args> (args)...)) {
	}

	// --------------------------------------------------------------------------------------------------------
	template <typename ... Args>
	exception::exception (exception cause, const char* function, const char* source, int line, Args&& ... args)
		: m_function (function),
		  m_source (source),
		  m_line (line),
		  m_text (_create (false, function, source, line, std::forward<Args> (args)...)),
		  m_chain (std::make_unique<exception> (std::move (cause))) { //-V1067
	}

	// --------------------------------------------------------------------------------------------------------
	template <typename ... Args>
	exception::exception ([[maybe_unused]] dummy_t custom, std::string prefix,
						  const char* function, const char* source, int line, Args&& ... args)
		:
		m_function (function),
		m_source (source),
		m_line (line),
		m_text (_create (false, function, source, line, prefix, std::forward<Args> (args)...)) {
	}

	// --------------------------------------------------------------------------------------------------------
	template <typename ... Args>
	inline
	std::string
	exception::_create (bool use_locus, const char* function, const char* source, int line, Args&& ... args) {
		std::ostringstream os;

		if (use_locus) {
			os << "Error at ";

			os << function << " " << _transform_source (source) << "@" << line;
		}
		if constexpr (sizeof...(args) > 0) {
			os << " :";
			((os << ' ' << std::forward<Args> (args)), ...);
		}
		return os.str ();
	}

	template <typename ... Args>
	inline
	void raise_exception [[noreturn]] (const char* function, const char* source, int line, Args&& ... args) {
		throw exception (function, source, line, std::forward<Args> (args)...);
	}

	template <typename ... Args>
	inline
	void
	raise_exception [[noreturn]] (exception cause, const char* function, const char* source, int line, Args&& ... args) {
		throw exception (std::move (cause), function, source, line, std::forward<Args> (args)...);
	}

	template <typename Exception>
	struct custom_raiser {
		template <typename ... Args>
		inline
		static
		void raise_exception [[noreturn]] (const char* function, const char* source, int line, Args&& ... args) {
			throw Exception (function, source, line, std::forward<Args> (args)...);
		}

		template <typename Cause, typename ... Args>
		inline
		static
		void
		raise_exception [[noreturn]] (Cause cause, const char* function, const char* source, int line, Args&& ... args) {
			throw Exception (std::move (cause), function, source, line, std::forward<Args> (args)...);
		}
	};
}

#if defined(_MSC_VER)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define RAISE_EX(...) ::bsw::raise_exception(__PRETTY_FUNCTION__, __FILE__, __LINE__,  ##__VA_ARGS__)
#define RAISE_EX_WITH_CAUSE(CAUSE, ...) ::bsw::raise_exception(CAUSE, __PRETTY_FUNCTION__, __FILE__, __LINE__,  ##__VA_ARGS__)

#define RAISE_EX_CUSTOM(Exception,...) ::bsw::custom_raiser<Exception>::raise_exception(__PRETTY_FUNCTION__, __FILE__, __LINE__,  ##__VA_ARGS__)
#define RAISE_EX_WITH_CAUSE_CUSTOM(Exception, CAUSE, ...) ::bsw::custom_raiser<Exception>::raise_exception(CAUSE, __PRETTY_FUNCTION__, __FILE__, __LINE__,  ##__VA_ARGS__)

#define ENFORCE(expr) while (!(expr)) {::bsw::raise_exception(__PRETTY_FUNCTION__, __FILE__, __LINE__, "Assertion failed: " STRINGIZE(expr));}
#endif
