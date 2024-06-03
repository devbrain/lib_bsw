//
// Created by igor on 3/1/24.
//
#include <algorithm>
#include <cstring>

#include <bsw/byte_order.hh>
#include <bsw/exception.hh>

#include <bsw/uuid.hh>

namespace bsw {
	uuid::uuid ()
		:
		_timeLow (0),
		_timeMid (0),
		_timeHiAndVersion (0),
		_clockSeq (0) {
		std::memset (_node, 0, sizeof (_node));
	}

	uuid::uuid (const uuid& uuid)
		:
		_timeLow (uuid._timeLow),
		_timeMid (uuid._timeMid),
		_timeHiAndVersion (uuid._timeHiAndVersion),
		_clockSeq (uuid._clockSeq) {
		std::memcpy (_node, uuid._node, sizeof (_node));
	}

	uuid::uuid (const std::string& auuid) {
		parse (auuid);
	}

	uuid::uuid (const char* auuid) {
		ENFORCE (auuid);
		parse (std::string (auuid));
	}
#include <bsw/warn/push.hh>
#include <bsw/warn/possible_loss_of_data_op>
	uuid::uuid (uint32_t timeLow, uint32_t timeMid, uint32_t timeHiAndVersion, uint16_t clockSeq, uint8_t node[])
		:
		_timeLow (timeLow),
		_timeMid (timeMid),
		_timeHiAndVersion (timeHiAndVersion),
		_clockSeq (clockSeq) {
		std::memcpy (_node, node, sizeof (_node));
	}
#include <bsw/warn/pop.hh>
	uuid::uuid (const char* bytes, version_t version) {
		uint32_t i32;
		uint16_t i16;
		std::memcpy (&i32, bytes, sizeof (i32));
		_timeLow = byte_order::from_network (i32);
		bytes += sizeof (i32);
		std::memcpy (&i16, bytes, sizeof (i16));
		_timeMid = byte_order::from_network (i16);
		bytes += sizeof (i16);
		std::memcpy (&i16, bytes, sizeof (i16));
		_timeHiAndVersion = byte_order::from_network (i16);
		bytes += sizeof (i16);
		std::memcpy (&i16, bytes, sizeof (i16));
		_clockSeq = byte_order::from_network (i16);
		bytes += sizeof (i16);
		std::memcpy (_node, bytes, sizeof (_node));

		_timeHiAndVersion &= 0x0FFF;
		_timeHiAndVersion |= (version << 12);
		_clockSeq &= 0x3FFF;
		_clockSeq |= 0x8000;
	}

	uuid::~uuid () = default;

	uuid& uuid::operator= (const uuid& auuid) {
		if (&auuid != this) {
			_timeLow = auuid._timeLow;
			_timeMid = auuid._timeMid;
			_timeHiAndVersion = auuid._timeHiAndVersion;
			_clockSeq = auuid._clockSeq;
			std::memcpy (_node, auuid._node, sizeof (_node));
		}
		return *this;
	}

	void uuid::swap (uuid& auuid) {
		std::swap (_timeLow, auuid._timeLow);
		std::swap (_timeMid, auuid._timeMid);
		std::swap (_timeHiAndVersion, auuid._timeHiAndVersion);
		std::swap (_clockSeq, auuid._clockSeq);
		std::swap_ranges (_node, _node + 6, &auuid._node[0]);
	}

	void uuid::parse (const std::string& auuid) {
		ENFORCE(try_parse (auuid))
	}

	bool uuid::try_parse (const std::string& auuid) {
		if (auuid.size () < 32) {
			return false;
		}

		bool haveHyphens = false;
		if (auuid[8] == '-' && auuid[13] == '-' && auuid[18] == '-' && auuid[23] == '-') {
			if (auuid.size () >= 36) {
				haveHyphens = true;
			} else {
				return false;
			}
		}

		uuid newUUID;
		std::string::const_iterator it = auuid.begin ();
		newUUID._timeLow = 0;
		for (int i = 0; i < 8; ++i) {
			int16_t n = nibble (*it++);
			if (n < 0) { return false; }
			newUUID._timeLow = (newUUID._timeLow << 4) | n;
		}
		if (haveHyphens) { ++it; }
		newUUID._timeMid = 0;
		for (int i = 0; i < 4; ++i) {
			int16_t n = nibble (*it++);
			if (n < 0) { return false; }
			newUUID._timeMid = (newUUID._timeMid << 4) | n;
		}
		if (haveHyphens) { ++it; }
		newUUID._timeHiAndVersion = 0;
		for (int i = 0; i < 4; ++i) {
			int16_t n = nibble (*it++);
			if (n < 0) { return false; }
			newUUID._timeHiAndVersion = (newUUID._timeHiAndVersion << 4) | n;
		}
		if (haveHyphens) { ++it; }
		newUUID._clockSeq = 0;
		for (int i = 0; i < 4; ++i) {
			int16_t n = nibble (*it++);
			if (n < 0) { return false; }
			newUUID._clockSeq = (newUUID._clockSeq << 4) | n;
		}
		if (haveHyphens) { ++it; }
		for (unsigned char & i : newUUID._node) {
			int16_t n1 = nibble (*it++);
			if (n1 < 0) { return false; }
			int16_t n2 = nibble (*it++);
			if (n2 < 0) { return false; }
#include <bsw/warn/push.hh>
#include <bsw/warn/possible_loss_of_data_op>
			i = (n1 << 4) | n2;
#include <bsw/warn/pop.hh>
		}
		swap (newUUID);

		return true;
	}

	std::string uuid::to_string () const {
		std::string result;
		result.reserve (36);
		appendHex (result, _timeLow);
		result += '-';
		append_hex (result, _timeMid);
		result += '-';
		append_hex (result, _timeHiAndVersion);
		result += '-';
		append_hex (result, _clockSeq);
		result += '-';
		for (unsigned char i : _node) {
			append_hex (result, i);
		}
		return result;
	}

	void uuid::copy_from (const char* buffer) {
		uint32_t i32;
		uint16_t i16;
		std::memcpy (&i32, buffer, sizeof (i32));
		_timeLow = byte_order::from_network (i32);
		buffer += sizeof (i32);
		std::memcpy (&i16, buffer, sizeof (i16));
		_timeMid = byte_order::from_network (i16);
		buffer += sizeof (i16);
		std::memcpy (&i16, buffer, sizeof (i16));
		_timeHiAndVersion = byte_order::from_network (i16);
		buffer += sizeof (i16);
		std::memcpy (&i16, buffer, sizeof (i16));
		_clockSeq = byte_order::from_network (i16);
		buffer += sizeof (i16);
		std::memcpy (_node, buffer, sizeof (_node));
	}

	void uuid::copy_to (char* buffer) const {
		uint32_t i32 = byte_order::to_network (_timeLow);
		std::memcpy (buffer, &i32, sizeof (i32));
		buffer += sizeof (i32);
		uint16_t i16 = byte_order::to_network (_timeMid);
		std::memcpy (buffer, &i16, sizeof (i16));
		buffer += sizeof (i16);
		i16 = byte_order::to_network (_timeHiAndVersion);
		std::memcpy (buffer, &i16, sizeof (i16));
		buffer += sizeof (i16);
		i16 = byte_order::to_network (_clockSeq);
		std::memcpy (buffer, &i16, sizeof (i16));
		buffer += sizeof (i16);
		std::memcpy (buffer, _node, sizeof (_node));
	}

	int uuid::variant () const {
		int v = _clockSeq >> 13;
		if ((v & 6) == 6) {
			return v;
		} else if (v & 4) {
			return 2;
		} else {
			return 0;
		}
	}

	int uuid::compare (const uuid& uuid) const {
		if (_timeLow != uuid._timeLow) { return _timeLow < uuid._timeLow ? -1 : 1; }
		if (_timeMid != uuid._timeMid) { return _timeMid < uuid._timeMid ? -1 : 1; }
		if (_timeHiAndVersion != uuid._timeHiAndVersion) { return _timeHiAndVersion < uuid._timeHiAndVersion ? -1 : 1; }
		if (_clockSeq != uuid._clockSeq) { return _clockSeq < uuid._clockSeq ? -1 : 1; }
		for (std::size_t i = 0; i < sizeof (_node); ++i) {
			if (_node[i] < uuid._node[i]) {
				return -1;
			} else if (_node[i] > uuid._node[i]) {
				return 1;
			}
		}
		return 0;
	}

	void uuid::append_hex (std::string& str, uint8_t n) {
		static const char* digits = "0123456789abcdef";
		str += digits[(n >> 4) & 0xF];
		str += digits[n & 0xF];
	}

	void uuid::append_hex (std::string& str, uint16_t n) {
		append_hex (str, uint8_t (n >> 8));
		append_hex (str, uint8_t (n & 0xFF));
	}

	void uuid::appendHex (std::string& str, uint32_t n) {
		append_hex (str, uint16_t (n >> 16));
		append_hex (str, uint16_t (n & 0xFFFF));
	}

	int16_t uuid::nibble (char hex) {
		if (hex >= 'a' && hex <= 'f') {
			return hex - 'a' + 10;
		} else if (hex >= 'A' && hex <= 'F') {
			return hex - 'A' + 10;
		} else if (hex >= '0' && hex <= '9') {
			return hex - '0';
		} else {
			return -1;
		}
	}

	void uuid::from_network () {
		_timeLow = byte_order::from_network (_timeLow);
		_timeMid = byte_order::from_network (_timeMid);
		_timeHiAndVersion = byte_order::from_network (_timeHiAndVersion);
		_clockSeq = byte_order::from_network (_clockSeq);
	}

	void uuid::to_network () {
		_timeLow = byte_order::to_network (_timeLow);
		_timeMid = byte_order::to_network (_timeMid);
		_timeHiAndVersion = byte_order::to_network (_timeHiAndVersion);
		_clockSeq = byte_order::to_network (_clockSeq);
	}

	namespace {
		uuid uuidNull;
		uuid uuidDNS ("6ba7b810-9dad-11d1-80b4-00c04fd430c8");
		uuid uuidURI ("6ba7b811-9dad-11d1-80b4-00c04fd430c8");
		uuid uuidOID ("6ba7b812-9dad-11d1-80b4-00c04fd430c8");
		uuid uuidX500 ("6ba7b814-9dad-11d1-80b4-00c04fd430c8");
	}

	const uuid& uuid::null () {
		return uuidNull;
	}

	const uuid& uuid::dns () {
		return uuidDNS;
	}

	const uuid& uuid::uri () {
		return uuidURI;
	}

	const uuid& uuid::oid () {
		return uuidOID;
	}

	const uuid& uuid::x500 () {
		return uuidX500;
	}

}