#ifndef BSW_LOGGER_ABSTRACT_PRINTER_HH
#define BSW_LOGGER_ABSTRACT_PRINTER_HH

#include <string>
#include <iosfwd>

#include <bsw/logger/logger_fwd.hh>

namespace bsw::logger {
	class abstract_printer {
	 public:
		abstract_printer ();
		virtual ~abstract_printer ();

		virtual void print_prologue (std::ostream& stream) = 0;
		virtual void print_epilogue (std::ostream& stream) = 0;

		virtual void before_log_record (std::ostream& stream, record_ptr_t record) = 0;
		virtual void after_log_record (std::ostream& stream, record_ptr_t record) = 0;
		virtual void print_function_name (std::ostream& stream, record_ptr_t record) = 0;
		virtual void print_space (std::ostream& stream, unsigned int count) = 0;
		virtual void print_level (std::ostream& stream, record_ptr_t record) = 0;
		virtual void print_file_name (std::ostream& stream, record_ptr_t record) = 0;
		virtual void print_line_number (std::ostream& stream, record_ptr_t record) = 0;
		virtual void print_thread_id (std::ostream& stream, record_ptr_t record) = 0;
		virtual void print_time_value (std::ostream& stream, record_ptr_t record) = 0;
		virtual void print_module_name (std::ostream& stream, record_ptr_t record) = 0;
		virtual void before_print_args (std::ostream& stream, record_ptr_t record) = 0;
		virtual void print_args (std::ostream& stream, record_ptr_t record) = 0;
		virtual void after_print_args (std::ostream& stream, record_ptr_t record) = 0;
		virtual void print_free_text (std::ostream& stream, const std::string& text) = 0;
	};
} // ns logger

#endif
