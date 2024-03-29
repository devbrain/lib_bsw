#ifndef BSW_LOGGER_LOCAL_DEVICE_HH
#define BSW_LOGGER_LOCAL_DEVICE_HH

#include <bsw/logger/device.hh>

namespace bsw::logger {
	class local_device : public basic_device {
	 public:
		local_device (formatter_ptr_t formatter, predicate_ptr_t predicate);
	 protected:
		[[nodiscard]] formatter_ptr_t _formatter () const;

		bool _do_open () override;
		virtual bool _open_device () = 0;

		bool _do_print (record_ptr_t record) override;
		virtual bool _do_print_string (const std::string& data) = 0;

		void _do_close () override;
		virtual void _close_device () = 0;

	 private:

		formatter_ptr_t m_formatter;

	};
} // ns logger


#endif 
