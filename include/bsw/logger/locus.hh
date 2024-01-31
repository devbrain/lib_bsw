#ifndef BSW_LOGGER_LOCUS_HH
#define BSW_LOGGER_LOCUS_HH

#include <bsw/export.h>

namespace bsw::logger {
	class BSW_EXPORT locus {
	 public:
		locus (const char* module_name,
			   const char* source,
			   const char* function,
			   unsigned int line);
		locus (const locus&) = default;
		~locus ();

		[[nodiscard]] const char* module () const;
		[[nodiscard]] const char* source () const;
		[[nodiscard]] const char* function () const;
		[[nodiscard]] unsigned int line () const;

	 private:
		char* m_module_name;
		char* m_src_file;
		char* m_function;
		unsigned int m_line;
		bool m_is_owner;
	};
} // ns logger



#endif
