#include <bsw/logger/device.hh>
#include <bsw/logger/formatter.hh>
#include <bsw/logger/record.hh>
#include <utility>

namespace bsw::logger {

	// =======================================================================
	basic_device::basic_device (predicate_ptr_t predicate)
		: m_predicate (std::move(predicate)),
		  m_is_opened (false) {
	}

	// ------------------------------------------------------------------------
	basic_device::~basic_device () = default;

	// -----------------------------------------------------------------------
	bool basic_device::open () {
		if (_do_open ()) {
			m_is_opened = true;
			return true;
		}
		return false;
	}

	// ------------------------------------------------------------------------
	bool basic_device::print (const record_ptr_t& record) {
		if (!_filter (record)) {
			return true;
		}
		return _do_print (record);
	}

	// ------------------------------------------------------------------------
	void basic_device::close () {
		if (!m_is_opened) {
			return;
		}
		_do_close ();
	}

	// ------------------------------------------------------------------------
	bool basic_device::is_opened () const {
		return m_is_opened;
	}

	// ------------------------------------------------------------------------
	bool basic_device::_filter (const record_ptr_t& record) {
		predicate_ptr_t pr = _predicate ();
		if (!pr) {
			return true;
		}
		return pr->call (record);
	}

	// -----------------------------------------------------------------------
	predicate_ptr_t basic_device::_predicate () const {
		return m_predicate;
	}

} // ns logger


