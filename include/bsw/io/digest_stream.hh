//
// Created by igor on 3/1/24.
//

#ifndef BSW_INCLUDE_BSW_IO_DIGEST_STREAM_HH_
#define BSW_INCLUDE_BSW_IO_DIGEST_STREAM_HH_

#include <bsw/export.h>
#include <iostream>
#include <bsw/io/buffered_stream_buf.hh>
#include <bsw/digest/digest_engine.hh>

namespace bsw::io {
	class BSW_EXPORT digest_buf : public buffered_stream_buf
		/// This streambuf computes a digest of all data going
		/// through it.
	{
		static constexpr std::size_t BUFFER_SIZE = 256;
	 public:
		explicit digest_buf (digest_engine& eng);
		digest_buf (digest_engine& eng, std::istream& istr);
		digest_buf (digest_engine& eng, std::ostream& ostr);
		~digest_buf () override;

		void close ();
	 private:
		int read_from_device (char* buffer, std::streamsize length) override;
		int write_to_device (const char* buffer, std::streamsize length) override;
	 private:
		digest_engine& _eng;
		std::istream* _pIstr;
		std::ostream* _pOstr;
	};

	class BSW_EXPORT digest_ios : public virtual std::ios
/// The base class for DigestInputStream and DigestOutputStream.
///
/// This class is needed to ensure the correct initialization
/// order of the stream buffer and base classes.
	{
	 public:
		explicit digest_ios (digest_engine& eng);
		digest_ios (digest_engine& eng, std::istream& istr);
		digest_ios (digest_engine& eng, std::ostream& ostr);
		~digest_ios () override;
		digest_buf* rdbuf ();

	 protected:
		digest_buf _buf;
	};

	class BSW_EXPORT digest_input_stream : public digest_ios, public std::istream
/// This istream computes a digest of
/// all the data passing through it,
/// using a digest_engine.
	{
	 public:
		digest_input_stream (digest_engine& eng, std::istream& istr);
		~digest_input_stream () override;
	};

	class BSW_EXPORT digest_output_stream : public digest_ios, public std::ostream
/// This ostream computes a digest of
/// all the data passing through it,
/// using a digest_engine.
/// To ensure that all data has been incorporated
/// into the digest, call close() or flush() before
/// you obtain the digest from the digest engine.
	{
	 public:
		explicit digest_output_stream (digest_engine& eng);
		digest_output_stream (digest_engine& eng, std::ostream& ostr);
		~digest_output_stream () override;
		void close ();
	};

}

#endif //BSW_INCLUDE_BSW_IO_DIGEST_STREAM_HH_
