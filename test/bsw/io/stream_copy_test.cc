//
// Created by igor on 2/29/24.
//

#include <doctest/doctest.h>
#include <bsw/io/stream_copier.hh>
#include <sstream>

using namespace bsw::io;

TEST_SUITE("stream copier") {
	TEST_CASE("buffered copy") {
		{
			std::string src;
			for (int i = 0; i < 255; ++i) { src += char (i); }
			std::istringstream istr (src);
			std::ostringstream ostr;
			std::streamsize n = stream_copier::copy_stream (istr, ostr);
			REQUIRE (ostr.str () == src);
			REQUIRE (n == src.size ());
		}
		{
			std::string src;
			for (int i = 0; i < 512; ++i) { src += char (i % 256); }
			std::istringstream istr (src);
			std::ostringstream ostr;
			std::streamsize n = stream_copier::copy_stream (istr, ostr, 100);
			REQUIRE (ostr.str () == src);
			REQUIRE (n == src.size ());
		}
		{
			std::string src;
			for (int i = 0; i < 512; ++i) { src += char (i % 256); }
			std::istringstream istr (src);
			std::ostringstream ostr;
			std::streamsize n = stream_copier::copy_stream (istr, ostr, 128);
			REQUIRE (ostr.str () == src);
			REQUIRE (n == src.size ());
		}
		{
			std::string src;
			for (int i = 0; i < 512; ++i) { src += char (i % 256); }
			std::istringstream istr (src);
			std::ostringstream ostr;
			std::streamsize n = stream_copier::copy_stream (istr, ostr, 512);
			REQUIRE (ostr.str () == src);
			REQUIRE (n == src.size ());
		}
	}

	TEST_CASE("copy to string") {
		std::string src;
		for (int i = 0; i < 512; ++i) { src += char (i % 256); }
		std::istringstream istr (src);
		std::string dest;
		std::streamsize n = stream_copier::copy_to_string (istr, dest, 100);
		REQUIRE (src == dest);
		REQUIRE (n == src.size ());
	}

	TEST_CASE("unbuffered copy") {
		std::string src;
		for (int i = 0; i < 255; ++i) { src += char (i); }
		std::istringstream istr (src);
		std::ostringstream ostr;
		std::streamsize n = stream_copier::copy_stream_unbuffered (istr, ostr);
		REQUIRE(ostr.str () == src);
		REQUIRE (n == src.size ());

	}
}