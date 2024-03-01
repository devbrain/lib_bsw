//
// Created by igor on 2/29/24.
//

#include <bsw/digest/digest_engine.hh>
#include <bsw/exception.hh>

namespace bsw {

	digest_engine::digest_engine () {
	}

	digest_engine::~digest_engine () {
	}

	std::string digest_engine::digest_to_hex (const digest_t& bytes) {
		static const char digits[] = "0123456789abcdef";
		std::string result;
		result.reserve (bytes.size () * 2);
		for (auto b : bytes) {
			result += digits[(b >> 4) & 0xF];
			result += digits[b & 0xF];
		}
		return result;
	}

	digest_engine::digest_t digest_engine::digest_from_hex (const std::string& digest) {
		if (digest.size () % 2 != 0) {

			RAISE_EX("Bad data format");
		}

		digest_t result;
		result.reserve (digest.size () / 2);
		for (std::size_t i = 0; i < digest.size (); ++i) {
			int c = 0;
			// first upper 4 bits
			if (digest[i] >= '0' && digest[i] <= '9') {
				c = digest[i] - '0';
			} else if (digest[i] >= 'a' && digest[i] <= 'f') {
				c = digest[i] - 'a' + 10;
			} else if (digest[i] >= 'A' && digest[i] <= 'F') {
				c = digest[i] - 'A' + 10;
			} else
				RAISE_EX("Bad data format");
			c <<= 4;
			++i;
			if (digest[i] >= '0' && digest[i] <= '9') {
				c += digest[i] - '0';
			} else if (digest[i] >= 'a' && digest[i] <= 'f') {
				c += digest[i] - 'a' + 10;
			} else if (digest[i] >= 'A' && digest[i] <= 'F') {
				c += digest[i] - 'A' + 10;
			} else
				RAISE_EX("Bad data format");;

			result.push_back (static_cast<unsigned char>(c));
		}
		return result;
	}

	bool digest_engine::constant_time_equals (const digest_t& d1, const digest_t& d2) {
		if (d1.size () != d2.size ()) { return false; }

		int result = 0;
		digest_t::const_iterator it1 = d1.begin ();
		digest_t::const_iterator it2 = d2.begin ();
		digest_t::const_iterator end1 = d1.end ();
		while (it1 != end1) {
			result |= *it1++ ^ *it2++;
		}
		return result == 0;
	}
}