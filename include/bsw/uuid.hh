//
// Created by igor on 3/1/24.
//

#ifndef BSW_INCLUDE_BSW_UUID_HH_
#define BSW_INCLUDE_BSW_UUID_HH_

#include <bsw/export.h>
#include <string>
#include <cstdint>

namespace bsw {
	class BSW_EXPORT uuid
		/// A UUID is an identifier that is unique across both space and time,
		/// with respect to the space of all UUIDs. Since a UUID is a fixed
		/// size and contains a time field, it is possible for values to
		/// rollover (around A.D. 3400, depending on the specific algorithm
		/// used). A UUID can be used for multiple purposes, from tagging
		/// objects with an extremely short lifetime, to reliably identifying
		/// very persistent objects across a network.
		///
		/// This class implements a Universal Unique Identifier,
		/// as specified in Appendix A of the DCE 1.1 Remote Procedure
		/// Call Specification (http://www.opengroup.org/onlinepubs/9629399/),
		/// RFC 2518 (WebDAV), section 6.4.1 and the UUIDs and GUIDs internet
		/// draft by Leach/Salz from February, 1998
		/// (http://www.ics.uci.edu/~ejw/authoring/uuid-guid/draft-leach-uuids-guids-01.txt)
		/// and also http://tools.ietf.org/html/draft-mealling-uuid-urn-05
	{
	 public:
		enum version_t {
			UUID_TIME_BASED = 0x01,
			UUID_DCE_UID = 0x02,
			UUID_NAME_BASED = 0x03,
			UUID_RANDOM = 0x04,
			UUID_NAME_BASED_SHA1 = 0x05

		};

		uuid ();
		/// Creates a nil (all zero) UUID.

		uuid (const uuid& uuid);
		/// Copy constructor.

		explicit uuid (const std::string& auuid);
		/// Parses the UUID from a string.

		explicit uuid (const char* auuid);
		/// Parses the UUID from a string.

		~uuid ();
		/// Destroys the UUID.

		uuid& operator= (const uuid& auuid);
		/// Assignment operator.

		void swap (uuid& auuid);
		/// Swaps the UUID with another one.

		void parse (const std::string& auuid);
		/// Parses the UUID from its string representation.

		bool try_parse (const std::string& auuid);
		/// Tries to interpret the given string as an UUID.
		/// If the UUID is syntactically valid, assigns the
		/// members and returns true. Otherwise leaves the
		/// object unchanged and returns false.

		[[nodiscard]] std::string to_string () const;
		/// Returns a string representation of the UUID consisting
		/// of groups of hexadecimal digits separated by hyphens.

		void copy_from (const char* buffer);
		/// Copies the UUID (16 bytes) from a buffer or byte array.
		/// The UUID fields are expected to be
		/// stored in network byte order.
		/// The buffer need not be aligned.

		void copy_to (char* buffer) const;
		/// Copies the UUID to the buffer. The fields
		/// are in network byte order.
		/// The buffer need not be aligned.
		/// There must have room for at least 16 bytes.

		[[nodiscard]] version_t version () const;
		/// Returns the version of the UUID.

		[[nodiscard]] int variant () const;
		/// Returns the variant number of the UUID:
		///   - 0 reserved for NCS backward compatibility
		///   - 2 the Leach-Salz variant (used by this class)
		///   - 6 reserved, Microsoft Corporation backward compatibility
		///   - 7 reserved for future definition

		bool operator== (const uuid& uuid) const;
		bool operator!= (const uuid& uuid) const;
		bool operator< (const uuid& uuid) const;
		bool operator<= (const uuid& uuid) const;
		bool operator> (const uuid& uuid) const;
		bool operator>= (const uuid& uuid) const;

		[[nodiscard]] bool is_null () const;
		/// Returns true iff the UUID is nil (in other words,
		/// consists of all zeros).

		static const uuid& null ();
		/// Returns a null/nil UUID.

		static const uuid& dns ();
		/// Returns the namespace identifier for the DNS namespace.

		static const uuid& uri ();
		/// Returns the namespace identifier for the URI (former URL) namespace.

		static const uuid& oid ();
		/// Returns the namespace identifier for the OID namespace.

		static const uuid& x500 ();
		/// Returns the namespace identifier for the X500 namespace.

	 protected:
		uuid (uint32_t timeLow, uint32_t timeMid, uint32_t timeHiAndVersion, uint16_t clockSeq, uint8_t node[]);
		uuid (const char* bytes, version_t version);
		[[nodiscard]] int compare (const uuid& uuid) const;
		static void append_hex (std::string& str, uint8_t n);
		static void append_hex (std::string& str, uint16_t n);
		static void appendHex (std::string& str, uint32_t n);
		static int16_t nibble (char hex);
		void from_network ();
		void to_network ();

	 private:
		uint32_t _timeLow;
		uint16_t _timeMid;
		uint16_t _timeHiAndVersion;
		uint16_t _clockSeq;
		uint8_t _node[6];

		friend class UUIDGenerator;
	};

//
// inlines
//
	inline bool uuid::operator== (const uuid& uuid) const {
		return compare (uuid) == 0;
	}

	inline bool uuid::operator!= (const uuid& uuid) const {
		return compare (uuid) != 0;
	}

	inline bool uuid::operator< (const uuid& uuid) const {
		return compare (uuid) < 0;
	}

	inline bool uuid::operator<= (const uuid& uuid) const {
		return compare (uuid) <= 0;
	}

	inline bool uuid::operator> (const uuid& uuid) const {
		return compare (uuid) > 0;
	}

	inline bool uuid::operator>= (const uuid& uuid) const {
		return compare (uuid) >= 0;
	}

	inline uuid::version_t uuid::version () const {
		return version_t (_timeHiAndVersion >> 12);
	}

	inline bool uuid::is_null () const {
		return compare (null ()) == 0;
	}

	inline void swap (uuid& u1, uuid& u2) {
		u1.swap (u2);
	}
}

#endif //BSW_INCLUDE_BSW_UUID_HH_
