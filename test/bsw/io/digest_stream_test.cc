//
// Created by igor on 3/1/24.
//
#include <sstream>
#include <doctest/doctest.h>

#include <bsw/io/digest_stream.hh>
#include <bsw/digest/md5.hh>

using namespace bsw::io;

TEST_SUITE("digest stream") {
	TEST_CASE("input stream") {
		std::istringstream istr ("abcdefghijklmnopqrstuvwxyz");
		bsw::md5_engine md5;
		digest_input_stream ds (md5, istr);
		std::string s;
		ds >> s;
		REQUIRE (bsw::digest_engine::digest_to_hex (md5.digest ()) == "c3fcd3d76192e4007dfb496cca67e13b");
		REQUIRE (s == "abcdefghijklmnopqrstuvwxyz");
	}

	TEST_CASE("output stream") {
		bsw::md5_engine md5;
		digest_output_stream ds(md5);
		ds << "abcdefghijklmnopqrstuvwxyz";
		ds.close();
		REQUIRE (bsw::digest_engine::digest_to_hex(md5.digest()) == "c3fcd3d76192e4007dfb496cca67e13b");

		ds << "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		ds << "abcdefghijklmnopqrstuvwxyz0123456789";
		ds.close();
		REQUIRE (bsw::digest_engine::digest_to_hex(md5.digest()) == "d174ab98d277d9f5a5611c2c9f419d9f");
	}

	TEST_CASE("output stream 2") {
		bsw::md5_engine md5;
		std::ostringstream ostr;
		digest_output_stream ds(md5, ostr);
		ds << "abcdefghijklmnopqrstuvwxyz";
		ds.close();
		REQUIRE (bsw::digest_engine::digest_to_hex(md5.digest()) == "c3fcd3d76192e4007dfb496cca67e13b");
		REQUIRE (ostr.str() == "abcdefghijklmnopqrstuvwxyz");
	}

	TEST_CASE("from hex") {
		std::string digest("c3fcd3d76192e4007dfb496cca67e13b");
		auto dig = bsw::digest_engine::digest_from_hex(digest);
		std::string digest2 = bsw::digest_engine::digest_to_hex(dig);
		REQUIRE (digest == digest2);
	}

}
