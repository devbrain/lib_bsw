#ifndef BSW_LOGGER_LAYOUT_HH
#define BSW_LOGGER_LAYOUT_HH

#include <string>
#include <iosfwd>

#include <list>

#include <bsw/logger/logger_fwd.hh>
#include <bsw/logger/layout_commands.hh>

namespace bsw::logger {

	class BSW_EXPORT applyer {
	 public:
		virtual ~applyer ();
		void apply (const printer_ptr_t& printer, std::ostream& stream, const record_ptr_t& record) const;
	 protected:
		void _add_command (const layout_command_ptr_t& cmd);
	 private:
		typedef std::list<layout_command_ptr_t> commands_list_t;
	 private:
#include <bsw/warn/push.hh>
#include <bsw/warn/dll_interface>

		commands_list_t m_commands;
#include <bsw/warn/pop.hh>

	};

	template <class Derived>
	class basic_layout_c : public applyer {
	 public:

		Derived* append_space (unsigned int count = 1);
		Derived* append_file_name ();
		Derived* append_func_name ();
		Derived* append_line_number ();
		Derived* append_thread_id ();
		Derived* append_time_value ();
		Derived* append_module_name ();
		Derived* append_text ();
		Derived* append_text (const std::string& text);
	};

} // ns logger


// ====================================================================
// Implementation
// ====================================================================

namespace bsw::logger {

	template <class Derived>
	Derived* basic_layout_c<Derived>::append_space (unsigned int count) {
		this->_add_command (layout_command_ptr_t (new space_command (count)));
		return dynamic_cast<Derived*>(this);
	}

	template <class Derived>
	Derived* basic_layout_c<Derived>::append_file_name () {
		this->_add_command (layout_command_ptr_t (new file_name_command));
		return dynamic_cast<Derived*>(this);
	}

	template <class Derived>
	Derived* basic_layout_c<Derived>::append_func_name () {
		this->_add_command (layout_command_ptr_t (new func_name_command));
		return dynamic_cast<Derived*>(this);
	}

	template <class Derived>
	Derived* basic_layout_c<Derived>::append_line_number () {
		this->_add_command (layout_command_ptr_t (new line_number_command));
		return dynamic_cast<Derived*>(this);
	}

	template <class Derived>
	Derived* basic_layout_c<Derived>::append_thread_id () {
		this->_add_command (layout_command_ptr_t (new thread_id_command));
		return dynamic_cast<Derived*>(this);
	}

	template <class Derived>
	Derived* basic_layout_c<Derived>::append_time_value () {
		this->_add_command (layout_command_ptr_t (new time_value_command));
		return dynamic_cast<Derived*>(this);
	}

	template <class Derived>
	Derived* basic_layout_c<Derived>::append_module_name () {
		this->_add_command (layout_command_ptr_t (new module_name_command));
		return dynamic_cast<Derived*>(this);
	}

	template <class Derived>
	Derived* basic_layout_c<Derived>::append_text () {
		this->_add_command (layout_command_ptr_t (new text_command));
		return dynamic_cast<Derived*>(this);
	}

	template <class Derived>
	Derived* basic_layout_c<Derived>::append_text (const std::string& text) {
		this->_add_command (layout_command_ptr_t (new free_text_command (text)));
		return dynamic_cast<Derived*>(this);
	}
} // ns logger

#endif


