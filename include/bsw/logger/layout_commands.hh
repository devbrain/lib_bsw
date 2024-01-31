#ifndef BSW_LOGGER_LAYOUT_COMMANDS_HH
#define BSW_LOGGER_LAYOUT_COMMANDS_HH

#include <string>
#include <bsw/logger/logger_fwd.hh>

namespace bsw::logger {
	class abstract_layout_command {
	 public:
		virtual ~abstract_layout_command ();
		virtual void call (printer_ptr_t printer, std::ostream& stream, record_ptr_t record) const = 0;
	};

	class space_command : public abstract_layout_command {
	 public:
		explicit space_command (unsigned int count);
		void call (printer_ptr_t printer, std::ostream& stream, record_ptr_t record) const override;
	 private:
		unsigned int m_count;
	};

	class file_name_command : public abstract_layout_command {
	 public:
		void call (printer_ptr_t printer, std::ostream& stream, record_ptr_t record) const override;
	};

	class func_name_command : public abstract_layout_command {
	 public:
		void call (printer_ptr_t printer, std::ostream& stream, record_ptr_t record) const override;
	};

	class line_number_command : public abstract_layout_command {
	 public:
		void call (printer_ptr_t printer, std::ostream& stream, record_ptr_t record) const override;
	};

	class thread_id_command : public abstract_layout_command {
	 public:
		void call (printer_ptr_t printer, std::ostream& stream, record_ptr_t record) const override;
	};

	class time_value_command : public abstract_layout_command {
	 public:
		void call (printer_ptr_t printer, std::ostream& stream, record_ptr_t record) const override;
	};

	class module_name_command : public abstract_layout_command {
	 public:
		void call (printer_ptr_t printer, std::ostream& stream, record_ptr_t record) const override;
	};

	class text_command : public abstract_layout_command {
	 public:
		void call (printer_ptr_t printer, std::ostream& stream, record_ptr_t record) const override;
	};

	class free_text_command : public abstract_layout_command {
	 public:
		explicit free_text_command (std::string txt);
		void call (printer_ptr_t printer, std::ostream& stream, record_ptr_t record) const override;
	 private:
		std::string m_txt;
	};
} // ns logger


#endif

