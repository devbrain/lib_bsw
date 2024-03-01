//
// Created by igor on 3/1/24.
//
#include <vector>
#include <cmath>
#include <doctest/doctest.h>
#include <bsw/io/random_stream.hh>
#include <bsw/random.hh>


using namespace bsw;
using namespace bsw::io;

TEST_SUITE("random") {
	TEST_CASE("Test stream") {
		random_input_stream rnd;

		const int n = 16;
		std::vector<int> d(n, 0);
		for (int i = 0; i < 1000; ++i)
		{
			unsigned char c;
			rnd >> c;
			d[c & 0x0F]++;
			d[(c >> 4) & 0x0F]++;
		}
		int sum = 0;
		for (int k = 0; k < n; ++k) sum += d[k];
		int avg = sum/n;
		int var = 0;
		for (int k = 0; k < n; ++k) var += (d[k] - avg)*(d[k] - avg);
		var /= n;
		int sd = int(std::sqrt((double) var));

		REQUIRE (((110 < avg) && (avg < 140)));
		REQUIRE (sd < 20);
	}

	TEST_CASE("seq1")
	{
		bsw::random rnd1;
		bsw::random rnd2;
		rnd1.seed(12345);
		rnd2.seed(12345);
		for (int i = 0; i < 100; ++i)
		{
			REQUIRE (rnd1.next() == rnd2.next());
		}
	}

	TEST_CASE("seq2")
	{
		bsw::random rnd1;
		bsw::random rnd2;
		rnd1.seed(12345);
		rnd2.seed(54321);

		bool equals = true;
		for (int i = 0; i < 20; ++i)
		{
			if (rnd1.next() != rnd2.next())
			{
				equals = false;
				break;
			}
		}
		REQUIRE (!equals);
	}


	TEST_CASE("test distribution 1")
	{
		bsw::random rnd;
		rnd.seed(123456);
		const int n = 11;
		int d[n] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		for (int i = 0; i < 100; ++i)
		{
			d[rnd.next() % n] = 1;
		}
		int sum = 0;
		for (int k = 0; k < n; ++k) sum += d[k];

		REQUIRE (sum == n);
	}


	TEST_CASE("test distribution 2")
	{
		bsw::random rnd;
		rnd.seed();
		const int n = 101;
		std::vector<int> d(n, 0);
		for (int i = 0; i < 10000; ++i)
		{
			d[rnd.next(n)]++;
		}
		int sum = 0;
		for (int k = 0; k < n; ++k) sum += d[k];
		int avg = sum/n;
		int var = 0;
		for (int k = 0; k < n; ++k) var += (d[k] - avg)*(d[k] - avg);
		var /= n;
		int sd = int(std::sqrt((double) var));

		REQUIRE ((95 < avg && avg < 105));
		REQUIRE (sd < 15);
	}


	TEST_CASE("test distribution 3")
	{
		bsw::random rnd;
		rnd.seed();
		const int n = 101;
		std::vector<int> d(n, 0);
		for (int i = 0; i < 10000; ++i)
		{
			d[int(rnd.next_float () * n)]++;
		}
		int sum = 0;
		for (int k = 0; k < n; ++k) sum += d[k];
		int avg = sum/n;
		int var = 0;
		for (int k = 0; k < n; ++k) var += (d[k] - avg)*(d[k] - avg);
		var /= n;
		int sd = int(std::sqrt((double) var));

		REQUIRE ((95 < avg && avg < 105));
		REQUIRE (sd < 15);
	}

}
