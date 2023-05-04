#ifndef BSW_BYTE_ORDER_HH
#define BSW_BYTE_ORDER_HH

#include <bsw/export.h>
#include <cstdint>
#include <bsw/bsw_config.h>

namespace bsw {

  class BSW_EXPORT byte_order {
    public:
      static int16_t flip_bytes (int16_t value);
      static uint16_t flip_bytes (uint16_t value);
      static int32_t flip_bytes (int32_t value);
      static uint32_t flip_bytes (uint32_t value);
      static uint64_t flip_bytes (uint64_t value);
      static int64_t flip_bytes (int64_t value);
      static int16_t to_big_endian (int16_t value);
      static uint16_t to_big_endian (uint16_t value);
      static int32_t to_big_endian (int32_t value);
      static uint32_t to_big_endian (uint32_t value);
      static int64_t to_big_endian (int64_t value);
      static uint64_t to_big_endian (uint64_t value);

      static int16_t from_big_endian (int16_t value);
      static uint16_t from_big_endian (uint16_t value);
      static int32_t from_big_endian (int32_t value);
      static uint32_t from_big_endian (uint32_t value);
      static int64_t from_big_endian (int64_t value);
      static uint64_t from_big_endian (uint64_t value);
      static int16_t to_little_endian (int16_t value);
      static uint16_t to_little_endian (uint16_t value);
      static int32_t to_little_endian (int32_t value);
      static uint32_t to_little_endian (uint32_t value);
      static int64_t to_little_endian (int64_t value);
      static uint64_t to_little_endian (uint64_t value);
      static int16_t from_little_endian (int16_t value);
      static uint16_t from_little_endian (uint16_t value);
      static int32_t from_little_endian (int32_t value);
      static uint32_t from_little_endian (uint32_t value);
      static int64_t from_little_endian (int64_t value);
      static uint64_t from_little_endian (uint64_t value);

      static int16_t to_network (int16_t value);
      static uint16_t to_network (uint16_t value);
      static int32_t to_network (int32_t value);
      static uint32_t to_network (uint32_t value);
      static int64_t to_network (int64_t value);
      static uint64_t to_network (uint64_t value);
      static int16_t from_network (int16_t value);
      static uint16_t from_network (uint16_t value);
      static int32_t from_network (int32_t value);
      static uint32_t from_network (uint32_t value);
      static int64_t from_network (int64_t value);
      static uint64_t from_network (uint64_t value);

  };

  //
  // inlines
  //
  inline uint16_t byte_order::flip_bytes (uint16_t value) {
    return static_cast <uint16_t>(((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00));
  }

  inline int16_t byte_order::flip_bytes (int16_t value) {
    return int16_t (flip_bytes (static_cast <uint16_t>(value)));
  }

  inline uint32_t byte_order::flip_bytes (uint32_t value) {
    return ((value >> 24) & 0x000000FF) | ((value >> 8) & 0x0000FF00)
           | ((value << 8) & 0x00FF0000) | ((value << 24) & 0xFF000000);
  }

  inline int32_t byte_order::flip_bytes (int32_t value) {
    return int32_t (flip_bytes (static_cast <uint32_t>(value)));
  }

  inline uint64_t byte_order::flip_bytes (uint64_t value) {
    const auto hi = uint32_t (value >> 32);
    const auto lo = uint32_t (value & 0xFFFFFFFF);
    return uint64_t (flip_bytes (hi)) | (uint64_t (flip_bytes (lo)) << 32);
  }

  inline int64_t byte_order::flip_bytes (int64_t value) {
    return int64_t (flip_bytes (uint64_t (value)));
  }

}  // ns

//
// some macro trickery to automate the method implementation
//
#define d_BSW_IMPLEMENT_BYTEORDER_NOOP_(op, type)            \
  inline type bsw::byte_order::op(type value)               \
  {                                                                     \
    return value;                                                       \
  }
#define d_BSW_IMPLEMENT_BYTEORDER_FLIP_(op, type)            \
  inline type bsw::byte_order::op(type value)               \
  {                                                                     \
    return flip_bytes(value);                                           \
  }

#define d_BSW_IMPLEMENT_BYTEORDER_NOOP(op)              \
    d_BSW_IMPLEMENT_BYTEORDER_NOOP_(op, int16_t)        \
    d_BSW_IMPLEMENT_BYTEORDER_NOOP_(op, uint16_t)       \
    d_BSW_IMPLEMENT_BYTEORDER_NOOP_(op, int32_t)        \
    d_BSW_IMPLEMENT_BYTEORDER_NOOP_(op, uint32_t)       \
    d_BSW_IMPLEMENT_BYTEORDER_NOOP_(op, int64_t)        \
    d_BSW_IMPLEMENT_BYTEORDER_NOOP_(op, uint64_t)

#define d_BSW_IMPLEMENT_BYTEORDER_FLIP(op)              \
    d_BSW_IMPLEMENT_BYTEORDER_FLIP_(op, int16_t)        \
    d_BSW_IMPLEMENT_BYTEORDER_FLIP_(op, uint16_t)       \
    d_BSW_IMPLEMENT_BYTEORDER_FLIP_(op, int32_t)        \
    d_BSW_IMPLEMENT_BYTEORDER_FLIP_(op, uint32_t)       \
    d_BSW_IMPLEMENT_BYTEORDER_FLIP_(op, int64_t)        \
    d_BSW_IMPLEMENT_BYTEORDER_FLIP_(op, uint64_t)

#if !defined(BSW_IS_LITTLE_ENDIAN)
#define d_BSW_IMPLEMENT_BYTEORDER_BIG d_BSW_IMPLEMENT_BYTEORDER_NOOP
#define d_BSW_IMPLEMENT_BYTEORDER_LIT d_BSW_IMPLEMENT_BYTEORDER_FLIP
#else
#define d_BSW_IMPLEMENT_BYTEORDER_BIG d_BSW_IMPLEMENT_BYTEORDER_FLIP
#define d_BSW_IMPLEMENT_BYTEORDER_LIT d_BSW_IMPLEMENT_BYTEORDER_NOOP
#endif

d_BSW_IMPLEMENT_BYTEORDER_BIG (to_big_endian)

d_BSW_IMPLEMENT_BYTEORDER_BIG (from_big_endian)

d_BSW_IMPLEMENT_BYTEORDER_BIG (to_network)

d_BSW_IMPLEMENT_BYTEORDER_BIG (from_network)

d_BSW_IMPLEMENT_BYTEORDER_LIT (to_little_endian)

d_BSW_IMPLEMENT_BYTEORDER_LIT (from_little_endian)

#endif 
