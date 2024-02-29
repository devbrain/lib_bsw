//
// Created by igor on 24/04/2022.
//

#ifndef INCLUDE_BSW_IO_COMPRESSION_TYPE_HH
#define INCLUDE_BSW_IO_COMPRESSION_TYPE_HH

namespace bsw::io {
  enum class compression_type_t {
      ZLIB,
      GZIP,
      ZLIB_RAW,
      ZSTD,
      BZ2,
      LZMA,
	  BROTLI
  };

  enum class compression_level_t {
    FAST,
    NORMAL,
    BEST
  };
}

#endif //INCLUDE_BSW_IO_COMPRESSION_TYPE_HH
