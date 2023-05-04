//
// Created by igor on 29/05/2022.
//

#include <doctest/doctest.h>
#include <bsw/id.hh>

TEST_CASE("test id") {
  auto id1 = bsw::id("AAA");
  auto id2 = bsw::id("PIZDA");
  auto id3 = bsw::id("AAA");
  REQUIRE(id1 == id3);
  REQUIRE(id2 != id1);
}