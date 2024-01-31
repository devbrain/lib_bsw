#include <iostream>
#include <fstream>

#include <bsw/logger/file_device.hh>
#include <bsw/logger/local_device.hh>
#include <bsw/logger/standard_layout.hh>
#include <bsw/logger/simple_printer.hh>
#include <bsw/logger/formatter.hh>
#include <bsw/logger/log_record_layout.hh>
#include <utility>

namespace bsw::logger {
	class file : public local_device {
	 public:
		file (formatter_ptr_t formatter, predicate_ptr_t predicate, std::filesystem::path  path);
	 private:
		bool _open_device () override;
		bool _do_print_string (const std::string& data) override;
		void _close_device () override;
	 private:
		std::filesystem::path m_path;
		std::ofstream* m_ofs;
	};

	file::file (formatter_ptr_t formatter, predicate_ptr_t predicate, std::filesystem::path  path)
		: local_device (std::move(formatter), std::move(predicate)),
		  m_path (std::move(path)),
		  m_ofs (nullptr) {
	}

	// -----------------------------------------------------------------
	bool file::_open_device () {
		m_ofs = new std::ofstream;

		m_ofs->open (m_path.u8string ());

		if (*m_ofs) {
			return true;
		}
		return false;
	}

	// -----------------------------------------------------------------
	bool file::_do_print_string (const std::string& data) {
		if (!data.empty ()) {
			(*m_ofs) << data << std::endl;
		}
		m_ofs->flush ();
		return true;
	}

	// -----------------------------------------------------------------
	void file::_close_device () {
		if (m_ofs) {
			m_ofs->flush ();
			delete m_ofs;
			m_ofs = nullptr;
		}
	}

	// -----------------------------------------------------------------
	device_ptr_t file_device (const std::filesystem::path& path, predicate_ptr_t predicate) {
		log_record_layout_ptr_t layout_ptr (standard_layout ());

		printer_ptr_t printer (new simple_printer);

		formatter_ptr_t fmatter (new formatter);

		fmatter->init (layout_ptr, printer);

		return std::make_shared<file> (fmatter, std::move(predicate), path);
	}

	// ----------------------------------------------------------------
	device_ptr_t file_device (const std::filesystem::path& path) {
		return file_device (path, nullptr);
	}
} // ns logger

