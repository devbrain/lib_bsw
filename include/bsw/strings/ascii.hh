//
// Created by igor on 20/03/2021.
//

#ifndef BSW_ASCII_HH
#define BSW_ASCII_HH

#include <bsw/export.h>
#include <cstdint>

namespace bsw {
  class BSW_EXPORT ascii
      /// This class contains enumerations and static
      /// utility functions for dealing with ASCII characters
      /// and their properties.
      ///
      /// The classification functions will also work if
      /// non-ASCII character codes are passed to them,
      /// but classification will only check for
      /// ASCII characters.
      ///
      /// This allows the classification methods to be used
      /// on the single bytes of a UTF-8 string, without
      /// causing assertions or inconsistent results (depending
      /// upon the current locale) on bytes outside the ASCII range,
      /// as may be produced by Ascii::isSpace(), etc.
  {
    public:
      enum character_properties : unsigned int
        /// ASCII character properties.
      {
        ACP_CONTROL = 0x0001u,
        ACP_SPACE = 0x0002u,
        ACP_PUNCT = 0x0004u,
        ACP_DIGIT = 0x0008u,
        ACP_HEXDIGIT = 0x0010u,
        ACP_ALPHA = 0x0020u,
        ACP_LOWER = 0x0040u,
        ACP_UPPER = 0x0080u,
        ACP_GRAPH = 0x0100u,
        ACP_PRINT = 0x0200u
      };

      [[nodiscard]] static unsigned int properties (int ch);
      /// Return the ASCII character properties for the
      /// character with the given ASCII value.
      ///
      /// If the character is outside the ASCII range
      /// (0 .. 127), 0 is returned.

      [[nodiscard]] static bool has_some_properties (int ch, unsigned int properties);
      /// Returns true if the given character is
      /// within the ASCII range and has at least one of
      /// the given properties.

      [[nodiscard]] static bool has_properties (int ch, unsigned int properties);
      /// Returns true if the given character is
      /// within the ASCII range and has all of
      /// the given properties.

      [[nodiscard]] static bool is_ascii (int ch);
      /// Returns true iff the given character code is within
      /// the ASCII range (0 .. 127).

      [[nodiscard]] static bool is_space (int ch);
      /// Returns true iff the given character is a whitespace.

      [[nodiscard]] static bool is_digit (int ch);
      /// Returns true iff the given character is a digit.

      [[nodiscard]] static bool is_hex_digit (int ch);
      /// Returns true iff the given character is a hexadecimal digit.

      [[nodiscard]] static bool is_punct (int ch);
      /// Returns true iff the given character is a punctuation character.

      [[nodiscard]] static bool is_alpha (int ch);
      /// Returns true iff the given character is an alphabetic character.

      [[nodiscard]] static bool is_alpha_numeric (int ch);
      /// Returns true iff the given character is an alphabetic character.

      [[nodiscard]] static bool is_lower (int ch);
      /// Returns true iff the given character is a lowercase alphabetic
      /// character.

      [[nodiscard]] static bool is_upper (int ch);
      /// Returns true iff the given character is an uppercase alphabetic
      /// character.

      [[nodiscard]] static bool is_printable (int ch);
      /// Returns true iff the given character is printable.

      [[nodiscard]] static int to_lower (int ch);
      /// If the given character is an uppercase character,
      /// return its lowercase counterpart, otherwise return
      /// the character.

      [[nodiscard]] static int to_upper (int ch);
      /// If the given character is a lowercase character,
      /// return its uppercase counterpart, otherwise return
      /// the character.

    private:
      static const unsigned int CHARACTER_PROPERTIES[128];
  };

//
// inlines
//
  inline unsigned int ascii::properties (int ch) {
    if (is_ascii (ch)) {
      return CHARACTER_PROPERTIES[ch];
    }
    else {
      return 0;
    }
  }

  // --------------------------------------------------------------------------------
  inline bool ascii::is_ascii (int ch) {
    return (static_cast<uint32_t>(ch) & 0xFFFFFF80) == 0;
  }

  // --------------------------------------------------------------------------------
  inline bool ascii::has_properties (int ch, unsigned int props) {
    return (properties (ch) & props) == props;
  }

  // --------------------------------------------------------------------------------
  inline bool ascii::has_some_properties (int ch, unsigned int props) {
    return (properties (ch) & props) != 0;
  }

  // --------------------------------------------------------------------------------
  inline bool ascii::is_space (int ch) {
    return has_properties (ch, ACP_SPACE);
  }

  // --------------------------------------------------------------------------------
  inline bool ascii::is_digit (int ch) {
    return has_properties (ch, ACP_DIGIT);
  }

  // --------------------------------------------------------------------------------
  inline bool ascii::is_hex_digit (int ch) {
    return has_properties (ch, ACP_HEXDIGIT);
  }

  // --------------------------------------------------------------------------------
  inline bool ascii::is_punct (int ch) {
    return has_properties (ch, ACP_PUNCT);
  }

  // --------------------------------------------------------------------------------
  inline bool ascii::is_alpha (int ch) {
    return has_properties (ch, ACP_ALPHA);
  }

  // --------------------------------------------------------------------------------
  inline bool ascii::is_alpha_numeric (int ch) {
    return has_some_properties (ch, ACP_ALPHA | ACP_DIGIT);
  }

  // --------------------------------------------------------------------------------
  inline bool ascii::is_lower (int ch) {
    return has_properties (ch, ACP_LOWER);
  }

  // --------------------------------------------------------------------------------
  inline bool ascii::is_upper (int ch) {
    return has_properties (ch, ACP_UPPER);
  }

  // --------------------------------------------------------------------------------
  inline bool ascii::is_printable (int ch) {
    return has_properties (ch, ACP_PRINT);
  }

  // --------------------------------------------------------------------------------
  inline int ascii::to_lower (int ch) {
    if (is_upper (ch)) {
      return ch + 32;
    }
    else {
      return ch;
    }
  }

  // --------------------------------------------------------------------------------
  inline int ascii::to_upper (int ch) {
    if (is_lower (ch)) {
      return ch - 32;
    }
    else {
      return ch;
    }
  }
}

#endif
