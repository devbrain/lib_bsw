#ifndef BSW_LOGGER_LOG_RECORD_LAYOUT_HH
#define BSW_LOGGER_LOG_RECORD_LAYOUT_HH

#include <bsw/logger/layout.hh>

namespace bsw::logger {

	class log_record_layout : public basic_layout_c<log_record_layout> {
	 public:
		~log_record_layout () override;
		log_record_layout* append_level ();
	};
} // ns logger


#endif
