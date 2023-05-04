//
// Created by igor on 24/04/2022.
//

#ifndef BSW_SRC_UTILS_IO_COMPRESSION_STREAM_BZIP_STREAM_HH
#define BSW_SRC_UTILS_IO_COMPRESSION_STREAM_BZIP_STREAM_HH


#include <io/compression_stream/compression_stream.hh>
#include <bsw/spimpl.h>

namespace bsw::io {
  namespace detail {
    struct bzip_impl;
  }


  class bzip_stream {
    public:
      bzip_stream();
      ~bzip_stream();

      [[nodiscard]] const uint8_t* next_in() const;
      [[nodiscard]] std::size_t avail_in() const;
      [[nodiscard]] uint8_t* next_out() const;
      [[nodiscard]] std::size_t avail_out() const;

      void set_next_in(const unsigned char* in);
      void set_avail_in(std::size_t in);
      void set_next_out(const uint8_t* in);
      void set_avail_out(std::size_t in);
    protected:
      spimpl::unique_impl_ptr<detail::bzip_impl> m_pimpl;
  };

  class bzip_compressor : public compression_stream_impl<bzip_stream> {
    public:
      explicit bzip_compressor(compression_level_t level);
      status_t compress(flush_mode_t flags) override;
      void finalize() override;
  };

  class bzip_decompressor : public decompression_stream_impl<bzip_stream> {
    public:
      bzip_decompressor();
      status_t decompress(flush_mode_t flags) override;
      void finalize() override;
      void reset() override;
  };
}


#endif //BSW_SRC_UTILS_IO_COMPRESSION_STREAM_BZIP_STREAM_HH
