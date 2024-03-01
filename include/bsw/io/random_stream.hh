//
// Created by igor on 3/1/24.
//

#ifndef BSW_INCLUDE_BSW_IO_RANDOM_STREAM_HH_
#define BSW_INCLUDE_BSW_IO_RANDOM_STREAM_HH_

#include <istream>
#include <bsw/io/buffered_stream_buf.hh>

namespace bsw::io {
	class BSW_EXPORT random_buf : public buffered_stream_buf
		/// This streambuf generates random data.
		/// On Windows NT, the cryptographic API is used.
		/// On Unix, /dev/random is used, if available.
		/// Otherwise, a random number generator, some
		/// more-or-less random data and a SHA-1 digest
		/// is used to generate random data.
	{
	 public:
		random_buf ();
		~random_buf () override;
	 private:
		int read_from_device (char* buffer, std::streamsize length) override;
	};

	class BSW_EXPORT random_ios : public virtual std::ios
/// The base class for RandomInputStream.
///
/// This class is needed to ensure the correct initialization
/// order of the stream buffer and base classes.
	{
	 public:
		random_ios ();
		~random_ios () override;
		random_buf* rdbuf ();

	 protected:
		random_buf _buf;
	};

	class BSW_EXPORT random_input_stream : public random_ios, public std::istream
/// This istream generates random data
/// using the RandomBuf.
	{
	 public:
		random_input_stream ();
		~random_input_stream () override;
	};

}

#endif //BSW_INCLUDE_BSW_IO_RANDOM_STREAM_HH_
