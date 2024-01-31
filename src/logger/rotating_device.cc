#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cstdio>

#include <bsw/logger/rotating_device.hh>
#include <bsw/logger/local_device.hh>
#include <bsw/logger/standard_layout.hh>
#include <bsw/logger/simple_printer.hh>
#include <bsw/logger/formatter.hh>
#include <bsw/logger/log_record_layout.hh>

#include <bsw/predef.h>

namespace bsw::logger {
	namespace priv {
		static int get_file_number (const std::string& name) {
			auto dot_idx = name.rfind ('.');
			if (dot_idx == std::string::npos) {
				return -1;
			}
			auto dash_idx = name.rfind ('-', dot_idx);
			if (dash_idx == std::string::npos) {
				return -1;
			}
			if (dash_idx + 1 == dot_idx) {
				return -1;
			}
			std::string material (name.c_str () + dash_idx + 1, name.c_str () + dot_idx);
			for (char ch : material) {
					if (ch < '0' || ch > '9') {
					return -1;
				}
			}
			std::istringstream is (material);
			int n;
			is >> n;
			return n;
		}

		// ==============================================================
		class rotating_device_c : public local_device {
		 public:
			rotating_device_c (formatter_ptr_t formatter, predicate_ptr_t predicate,
							   const std::filesystem::path& prefix, unsigned num, std::size_t max_lines);
		 private:
			bool _open_device () override;
			bool _do_print_string (const std::string& data) override;
			void _close_device () override;
		 private:
			struct file_name_s {
				file_name_s (unsigned n_, std::filesystem::path name_)
					: n (n_),
					  name (std::move (name_)) {

				}

				unsigned n;
				std::filesystem::path name;
			};

		 private:
			bool _start_pool ();
			bool _rotate_log ();
		 private:
			std::filesystem::path m_path;
			std::string m_base_name;
			std::string m_ext;
			unsigned m_num;
			unsigned m_start_seq;
			std::vector<file_name_s> m_pool;
			std::size_t m_pool_idx;
			FILE* m_current_device;
			std::size_t m_max_lines;
			std::size_t m_lines;
		};

		// ========================================================
		rotating_device_c::rotating_device_c (formatter_ptr_t formatter, predicate_ptr_t predicate,
											  const std::filesystem::path& prefix, unsigned num, std::size_t max_lines)
			: local_device (std::move(formatter), std::move(predicate)),
			  m_num (num),
			  m_start_seq (0),
			  m_pool_idx (0),
			  m_current_device (nullptr),
			  m_max_lines (max_lines),
			  m_lines (0) {
			m_base_name = prefix.filename ().u8string ();
			m_path = prefix.parent_path ();

			auto idx = m_base_name.rfind ('.');
			if (idx != std::string::npos) {
#include "bsw/warn/push.hh"
#include "bsw/warn/sign_conversion"

				m_ext.assign (m_base_name.c_str () + idx + 1);
				m_base_name.erase (m_base_name.begin () + idx, m_base_name.end ());
#include "bsw/warn/pop.hh"
			}
		}

		// ------------------------------------------------------
		bool rotating_device_c::_open_device () {
			std::vector<file_name_s> candidates;

			for (const auto& entry : std::filesystem::directory_iterator (m_path)) {
				const std::filesystem::path& path = entry.path ();
				if (!std::filesystem::is_directory (path)) {
					auto obj_name = path.filename ().u8string ();
					auto idx = obj_name.find (m_base_name);
					if (idx == 0) {
						int n = get_file_number (obj_name);
						if (n > -1) {
							candidates.emplace_back(static_cast<unsigned>(n), obj_name);
						}
					}
				}
			}

			if (!candidates.empty ()) {
				std::sort (candidates.begin (), candidates.end (),
						   [] (const file_name_s& a, const file_name_s& b) {
							 return a.n < b.n;
						   }
				);
				m_start_seq = candidates.back ().n;
			}
			m_start_seq++;
			unsigned cur_seq = m_start_seq;
			if (candidates.empty ()) {
				cur_seq = 0;
			}
			bool has_empty = false;
			m_pool_idx = 0;
			for (std::size_t i = 0; i < m_num; i++) {
				if (i < candidates.size ()) {
					auto log_seq = candidates[i];
					cur_seq = log_seq.n;
					m_pool.push_back (log_seq);
					m_pool_idx++;
				} else {

					file_name_s log_seq (++cur_seq, "");
					m_pool.push_back (log_seq);
					if (!has_empty) {
						has_empty = true;
						m_pool_idx++;
					}
				}
			}
			if (m_pool_idx > 0) {
				m_pool_idx--;
			}
			if (!has_empty) {
				m_start_seq--;
				return _rotate_log ();
			}
			return _start_pool ();
		}

		// ----------------------------------------------------------
		bool rotating_device_c::_start_pool () {
			auto log_seq = m_pool[m_pool_idx];
			std::ostringstream os;
			os << m_base_name << "-" << log_seq.n << "." << m_ext;
			auto new_fname = m_path / std::filesystem::path (os.str ());
			auto device = fopen (new_fname.u8string ().c_str (), "wb");
			if (device) {
				if (m_current_device) {
					fclose (m_current_device);
				}

				m_current_device = device;

				if (log_seq.name.empty ()) {
					os.str ("");
					os.clear ();

					auto old_fname = m_path / log_seq.name;

					if (std::filesystem::exists (old_fname) && !std::filesystem::is_directory (old_fname)) {
						std::filesystem::remove (old_fname);
					}
				}
				m_pool[m_pool_idx].name = new_fname;
			}
			m_lines = 0;
			return device != nullptr;
		}

		// ----------------------------------------------------------
		bool rotating_device_c::_rotate_log () {
			m_pool_idx++;
			if (m_pool_idx >= m_pool.size ()) {
				m_pool_idx = 0;
			}
			m_pool[m_pool_idx].n = ++m_start_seq;

			return _start_pool ();
		}

		// ----------------------------------------------------------
		bool rotating_device_c::_do_print_string (const std::string& data) {
			if (data.empty ()) {
				return true;
			}

			if (m_current_device) {

				if (m_lines >= m_max_lines) {
					_rotate_log ();
				}
				m_lines++;
				fwrite (data.c_str (), 1, data.size (), m_current_device);
				std::string x = "\n\r";
				fwrite (x.c_str (), 1, x.size (), m_current_device);
				fflush (m_current_device);
				return true;
			}
			return false;
		}

		// ----------------------------------------------------------
		void rotating_device_c::_close_device () {
			if (m_current_device) {
				fclose (m_current_device);
			}
		}
	} // ns priv

	// ----------------------------------------------------------
	device_ptr_t
	rotating_device (const std::filesystem::path& prefix, unsigned num, std::size_t max_lines, predicate_ptr_t predicate) {
		log_record_layout_ptr_t layout_ptr (standard_layout ());

		printer_ptr_t printer (new simple_printer);

		formatter_ptr_t fmatter (new formatter);

		fmatter->init (layout_ptr, printer);

		device_ptr_t dev (new priv::rotating_device_c (fmatter, std::move(predicate), prefix, num, max_lines));
		return dev;
	}

	// ----------------------------------------------------------
	device_ptr_t rotating_device (const std::filesystem::path& prefix, unsigned num, std::size_t max_lines) {
		predicate_ptr_t predicate = nullptr;
		return rotating_device (prefix, num, max_lines, predicate);
	}
} // ns logger

