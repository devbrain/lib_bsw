//
// Created by igor on 2/29/24.
//
#include <doctest/doctest.h>
#include <sstream>
#include <bsw/io/tee_stream.hh>

TEST_SUITE("tee stream") {
	TEST_CASE("input stream") {
		std::istringstream istr ("foo");
		std::ostringstream ostr;
		bsw::io::tee_input_stream tis (istr);
		tis.add_stream (ostr);
		std::string s;
		tis >> s;
		REQUIRE (ostr.str () == "foo");
	}

	TEST_CASE("output stream") {
		std::ostringstream ostr1;
		std::ostringstream ostr2;
		bsw::io::tee_output_stream tos (ostr1);
		tos.add_stream (ostr2);
		tos << "bar" << std::flush;
		REQUIRE (ostr1.str () == "bar");
		REQUIRE (ostr2.str () == "bar");
	}
}