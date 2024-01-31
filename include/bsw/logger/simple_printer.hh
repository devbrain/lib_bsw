#ifndef BSW_LOGGER_SIMPLE_PRINTER_HH
#define BSW_LOGGER_SIMPLE_PRINTER_HH

#include <bsw/logger/abstract_printer.hh>

namespace bsw::logger {
	class simple_printer : public abstract_printer {
	 private:
		void print_function_name (std::ostream& stream, record_ptr_t record) override;
		void print_space (std::ostream& stream, unsigned int count) override;
		void print_level (std::ostream& stream, record_ptr_t record) override;
		void print_file_name (std::ostream& stream, record_ptr_t record) override;
		void print_line_number (std::ostream& stream, record_ptr_t record) override;
		void print_thread_id (std::ostream& stream, record_ptr_t record) override;
		void print_time_value (std::ostream& stream, record_ptr_t record) override;
		void print_module_name (std::ostream& stream, record_ptr_t record) override;
		void print_args (std::ostream& stream, record_ptr_t record) override;
		void print_free_text (std::ostream& stream, const std::string& text) override;

		void print_prologue (std::ostream& stream) override;
		void print_epilogue (std::ostream& stream) override;
		void before_log_record (std::ostream& stream, record_ptr_t record) override;
		void after_log_record (std::ostream& stream, record_ptr_t record) override;
		void before_print_args (std::ostream& stream, record_ptr_t record) override;
		void after_print_args (std::ostream& stream, record_ptr_t record) override;
	};
} // ns logger


#endif
