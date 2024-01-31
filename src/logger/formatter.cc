#include <bsw/logger/formatter.hh>
#include <bsw/logger/log_record_layout.hh>
#include <bsw/logger/abstract_printer.hh>
#include <utility>

namespace bsw::logger {
	formatter::formatter () = default;

	// ---------------------------------------------------------
	formatter::~formatter () = default;

	// ---------------------------------------------------------
	void formatter::init (log_record_layout_ptr_t layout, printer_ptr_t printer) {
		m_layout = std::move(layout);
		m_printer = std::move(printer);
	}

	// ---------------------------------------------------------
	void formatter::start (std::ostream& stream) {
		m_printer->print_prologue (stream);
	}

	// ---------------------------------------------------------
	void formatter::done (std::ostream& stream) {
		m_printer->print_epilogue (stream);
	}

	// ---------------------------------------------------------
	void formatter::print (const record_ptr_t& record, std::ostream& stream) {
		m_printer->before_log_record (stream, record);
		m_layout->apply (m_printer, stream, record);
		m_printer->after_log_record (stream, record);
	}
} // ns logger


