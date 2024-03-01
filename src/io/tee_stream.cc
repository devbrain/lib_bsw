//
// Created by igor on 2/29/24.
//

#include <bsw/io/tee_stream.hh>
#include "bsw/io/ios_init.hh"
namespace bsw::io {

	tee_stream_buf::tee_stream_buf ()
		:
		m_istr (0) {
	}

	tee_stream_buf::tee_stream_buf (std::istream& istr)
		:
		m_istr (&istr) {
	}

	tee_stream_buf::tee_stream_buf (std::ostream& ostr)
		:
		m_istr (0) {
		m_streams.push_back (&ostr);
	}

	tee_stream_buf::~tee_stream_buf () {
	}

	void tee_stream_buf::add_stream (std::ostream& ostr) {
		m_streams.push_back (&ostr);
	}

	int tee_stream_buf::read_from_device () {
		if (m_istr) {
			int c = m_istr->get ();
			if (c != -1) { write_to_device ((char)c); }
			return c;
		}
		return -1;
	}

	int tee_stream_buf::write_to_device (char c) {
		for (auto& pStream : m_streams) {
			pStream->put (c);
		}
		return char_to_int (c);
	}

	tee_ios::tee_ios () {
		bsw_ios_init (&m_buf);
	}

	tee_ios::tee_ios (std::istream& istr)
		: m_buf (istr) {
		bsw_ios_init (&m_buf);
	}

	tee_ios::tee_ios (std::ostream& ostr)
		: m_buf (ostr) {
		bsw_ios_init (&m_buf);
	}

	tee_ios::~tee_ios () {
	}

	void tee_ios::add_stream (std::ostream& ostr) {
		m_buf.add_stream (ostr);
	}

	tee_stream_buf* tee_ios::rdbuf () {
		return &m_buf;
	}

	tee_input_stream::tee_input_stream (std::istream& istr)
		: tee_ios (istr), std::istream (&m_buf) {
	}

	tee_input_stream::~tee_input_stream () {
	}

	tee_output_stream::tee_output_stream ()
		: std::ostream (&m_buf) {
	}

	tee_output_stream::tee_output_stream (std::ostream& ostr)
		: tee_ios (ostr), std::ostream (&m_buf) {
	}

	tee_output_stream::~tee_output_stream () {
	}
}