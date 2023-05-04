//
// Created by igor on 06/06/2021.
//

#include <doctest/doctest.h>
#include <tuple>
#include <string>
#include <bsw/mp/constexpr_for.hh>
#include <bsw/mp/typelist.hh>

TEST_SUITE("constexpr_for")
{
    TEST_CASE("tuple")
    {
        auto t = std::make_tuple(1, 3.14, std::string("ZOPA"));
        using T = decltype(t);
        bsw::mp::constexpr_for<0, std::tuple_size_v<T>, 1>([&t](auto idx) {
            auto x = std::get<idx.value>(t);
            if constexpr(idx.value == 0) {
                REQUIRE(x == 1);
            } else if constexpr(idx.value == 1) {
                REQUIRE(x == 3.14);
            } else if constexpr(idx.value == 1) {
                REQUIRE(x == "ZOPA");
            }
        });
    }

    TEST_CASE("typelist")
    {
        using T = bsw::mp::type_list<bool, int, double>;

        bsw::mp::constexpr_for<0, T::size(), 1>([](auto idx) {
            using x = bsw::mp::type_list_at_t<idx.value, T>;
            if constexpr(idx.value == 0) {
                REQUIRE(std::is_same_v<x, bool>);
            } else if constexpr(idx.value == 1) {
                    REQUIRE(std::is_same_v<x, int>);
                } else if constexpr(idx.value == 1) {
                        REQUIRE(std::is_same_v<x, double>);
                    }
        });
    }
}

