//
// Created by igor on 3/1/24.
//

#include <bsw/io/digest_stream.hh>
#include <bsw/io/ios_init.hh>

namespace bsw::io {

	digest_buf::digest_buf (digest_engine& eng)
		:
		buffered_stream_buf (BUFFER_SIZE, std::ios::out),
		_eng (eng),
		_pIstr (nullptr),
		_pOstr (nullptr) {
	}

	digest_buf::digest_buf (digest_engine& eng, std::istream& istr)
		:
		buffered_stream_buf (BUFFER_SIZE, std::ios::in),
		_eng (eng),
		_pIstr (&istr),
		_pOstr (nullptr) {
	}

	digest_buf::digest_buf (digest_engine& eng, std::ostream& ostr)
		:
		buffered_stream_buf (BUFFER_SIZE, std::ios::out),
		_eng (eng),
		_pIstr (nullptr),
		_pOstr (&ostr) {
	}

	digest_buf::~digest_buf () = default;

	int digest_buf::read_from_device (char* buffer, std::streamsize length) {
		if (_pIstr && _pIstr->good ()) {
			_pIstr->read (buffer, length);
			std::streamsize n = _pIstr->gcount ();
			if (n > 0) { _eng.update (buffer, static_cast<unsigned>(n)); }
			return static_cast<int>(n);
		}
		return -1;
	}

	int digest_buf::write_to_device (const char* buffer, std::streamsize length) {
		_eng.update (buffer, (unsigned)length);
		if (_pOstr) { _pOstr->write (buffer, length); }
		return static_cast<int>(length);
	}

	void digest_buf::close () {
		sync ();
		if (_pOstr) { _pOstr->flush (); }
	}

	digest_ios::digest_ios (digest_engine& eng)
		: _buf (eng) {
		bsw_ios_init(&_buf);
	}

	digest_ios::digest_ios (digest_engine& eng, std::istream& istr)
		: _buf (eng, istr) {
		bsw_ios_init(&_buf);
	}

	digest_ios::digest_ios (digest_engine& eng, std::ostream& ostr)
		: _buf (eng, ostr) {
		bsw_ios_init(&_buf);
	}

	digest_ios::~digest_ios () = default;

	digest_buf* digest_ios::rdbuf () {
		return &_buf;
	}

	digest_input_stream::digest_input_stream (digest_engine& eng, std::istream& istr)
		:
		digest_ios (eng, istr),
		std::istream (&_buf) {
	}

	digest_input_stream::~digest_input_stream () = default;

	digest_output_stream::digest_output_stream (digest_engine& eng)
		:
		digest_ios (eng),
		std::ostream (&_buf) {
	}

	digest_output_stream::digest_output_stream (digest_engine& eng, std::ostream& ostr)
		:
		digest_ios (eng, ostr),
		std::ostream (&_buf) {
	}

	digest_output_stream::~digest_output_stream () = default;

	void digest_output_stream::close () {
		_buf.close ();
	}

}