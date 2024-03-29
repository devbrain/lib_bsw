#include "bsw/logger/logger_helper.hh"
#include "bsw/logger/system.hh"

static bool disabled = false;

namespace bsw::logger {
	void disable () {
		disabled = true;
	}

	void post_record (const std::shared_ptr<logger::record>& record) {
		if (!disabled) {
			if (record->level () == level_t::eFATAL) {
				//crash::generate_dump();
			}
			post (record);
		}
	}
} // ns logger

