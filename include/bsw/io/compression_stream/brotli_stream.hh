//
// Created by igor on 2/29/24.
//

#ifndef BSW_INCLUDE_BSW_IO_COMPRESSION_STREAM_BROTLI_STREAM_HH_
#define BSW_INCLUDE_BSW_IO_COMPRESSION_STREAM_BROTLI_STREAM_HH_

#include <bsw/export.h>
#include <bsw/io/compression_stream/compression_stream.hh>
#include <bsw/spimpl.h>

namespace bsw::io {
	namespace detail {
		struct brotli_impl;
	}


	class BSW_EXPORT brotli_stream {
	 public:
		brotli_stream();
		~brotli_stream();

		[[nodiscard]] const uint8_t* next_in() const;
		[[nodiscard]] std::size_t avail_in() const;
		[[nodiscard]] uint8_t* next_out() const;
		[[nodiscard]] std::size_t avail_out() const;

		void set_next_in(const unsigned char* in);
		void set_avail_in(std::size_t in);
		void set_next_out(const uint8_t* in);
		void set_avail_out(std::size_t in);
	 protected:
		spimpl::unique_impl_ptr<detail::brotli_impl> m_pimpl;
	};

	class BSW_EXPORT brotli_compressor : public compression_stream_impl<brotli_stream> {
	 public:
		brotli_compressor();
		~brotli_compressor() override;
		status_t compress(flush_mode_t flags) override;
		void finalize() override;
	};

	class BSW_EXPORT brotli_decompressor : public decompression_stream_impl<brotli_stream> {
	 public:
		brotli_decompressor();
		~brotli_decompressor() override;

		status_t decompress(flush_mode_t flags) override;
		void finalize() override;
		void reset() override;
	};
}

#endif //BSW_INCLUDE_BSW_IO_COMPRESSION_STREAM_BROTLI_STREAM_HH_
