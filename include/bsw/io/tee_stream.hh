//
// Created by igor on 2/29/24.
//

#ifndef BSW_INCLUDE_BSW_IO_TEE_STREAM_HH_
#define BSW_INCLUDE_BSW_IO_TEE_STREAM_HH_

#include <vector>
#include <istream>
#include <ostream>
#include <bsw/io/unbuffered_stream_buf.hh>
#include <bsw/export.h>

namespace bsw::io {

	class BSW_EXPORT tee_stream_buf : public unbuffered_stream_buf
		/// This stream buffer copies all data written to or
		/// read from it to one or multiple output streams.
	{
	 public:
		tee_stream_buf ();
		/// Creates an unconnected CountingStreamBuf.
		/// Use add_stream() to attach output streams.

		explicit tee_stream_buf (std::istream& istr);
		/// Creates the CountingStreamBuf and connects it
		/// to the given input stream.

		explicit tee_stream_buf (std::ostream& ostr);
		/// Creates the CountingStreamBuf and connects it
		/// to the given output stream.

		~tee_stream_buf () override;
		/// Destroys the CountingStream.

		void add_stream (std::ostream& ostr);
		/// Adds the given output stream.

	 protected:
		int read_from_device () override;
		int write_to_device (char c) override;

	 private:
		using stream_vec_t = std::vector<std::ostream*>;

		std::istream* m_istr;
		stream_vec_t m_streams;
	};

	class BSW_EXPORT tee_ios : public virtual std::ios
/// The base class for TeeInputStream and TeeOutputStream.
///
/// This class is needed to ensure the correct initialization
/// order of the stream buffer and base classes.
	{
	 public:
		tee_ios ();
/// Creates the basic stream and leaves it unconnected.

		explicit tee_ios (std::istream& istr);
/// Creates the basic stream and connects it
/// to the given input stream.

		explicit tee_ios (std::ostream& ostr);
/// Creates the basic stream and connects it
/// to the given output stream.

		~tee_ios () override;
/// Destroys the stream.

		void add_stream (std::ostream& ostr);
/// Adds the given output stream.

		tee_stream_buf* rdbuf ();
/// Returns a pointer to the underlying streambuf.

	 protected:
		tee_stream_buf m_buf;
	};

	class BSW_EXPORT tee_input_stream : public tee_ios, public std::istream
/// This stream copies all characters read through it
/// to one or multiple output streams.
	{
	 public:
		explicit tee_input_stream (std::istream& istr);
/// Creates the TeeInputStream and connects it
/// to the given input stream.

		~tee_input_stream () override;
/// Destroys the TeeInputStream.
	};

	class BSW_EXPORT tee_output_stream : public tee_ios, public std::ostream
/// This stream copies all characters written to it
/// to one or multiple output streams.
	{
	 public:
		tee_output_stream ();
/// Creates an unconnected TeeOutputStream.

		explicit tee_output_stream (std::ostream& ostr);
/// Creates the TeeOutputStream and connects it
/// to the given input stream.

		~tee_output_stream () override;
/// Destroys the TeeOutputStream.
	};

} // namespace Poco


#endif //BSW_INCLUDE_BSW_IO_TEE_STREAM_HH_
