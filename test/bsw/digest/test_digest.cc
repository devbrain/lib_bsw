//
// Created by igor on 2/29/24.
//

#include <doctest/doctest.h>
#include <bsw/digest/md4.hh>
#include <bsw/digest/md5.hh>
#include <bsw/digest/sha1.hh>
#include <bsw/digest/sha2.hh>
#include <bsw/digest/crc32.hh>

using namespace bsw;

TEST_SUITE("digest test") {
	TEST_CASE("crc32") {
		crc32_engine engine;
		engine.update ("");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "00000000");
		engine.update ("a");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "e8b7be43");
		engine.update ("abc");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "352441c2");
		engine.update ("message digest");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "20159d7f");
		engine.update ("abcdefghijklmnopqrstuvwxyz");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "4c2750bd");
		engine.update ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "1fc2e6d2");
		engine.update ("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "7ca94a72");
	}
	TEST_CASE("md4") {
		md4_engine engine;

		// test vectors from RFC 1320

		engine.update ("");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "31d6cfe0d16ae931b73c59d7e0c089c0");

		engine.update ("a");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "bde52cb31de33e46245e05fbdbd6fb24");

		engine.update ("abc");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "a448017aaf21d8525fc10ae87aa6729d");

		engine.update ("message digest");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "d9130a8164549fe818874806e1c7014b");

		engine.update ("abcdefghijklmnopqrstuvwxyz");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "d79e1c308aa5bbcdeea8ed63df412da9");

		engine.update ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		engine.update ("abcdefghijklmnopqrstuvwxyz0123456789");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "043f8582f241db351ce627e153e7f0e4");

		engine.update ("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "e33b4ddc9c38f2199c3e7b164fcc0536");
	}

	TEST_CASE("md5") {
		md5_engine engine;
		engine.update ("");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "d41d8cd98f00b204e9800998ecf8427e");

		engine.update ("a");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "0cc175b9c0f1b6a831c399e269772661");

		engine.update ("abc");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "900150983cd24fb0d6963f7d28e17f72");

		engine.update ("message digest");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "f96b697d7cb7938d525a2f31aaf161d0");

		engine.update ("abcdefghijklmnopqrstuvwxyz");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "c3fcd3d76192e4007dfb496cca67e13b");

		engine.update ("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		engine.update ("abcdefghijklmnopqrstuvwxyz0123456789");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "d174ab98d277d9f5a5611c2c9f419d9f");

		engine.update ("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "57edf4a22be3c955ac49da2e2107b67a");
	}

	TEST_CASE("sha-1") {
		sha1_engine engine;

		// test vectors from FIPS 180-1

		engine.update ("abc");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "a9993e364706816aba3e25717850c26c9cd0d89d");

		engine.update ("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "84983e441c3bd26ebaae4aa1f95129e5e54670f1");

		for (int i = 0; i < 1000000; ++i) {
			engine.update ('a');
		}
		REQUIRE (digest_engine::digest_to_hex (engine.digest ()) == "34aa973cd4c4daa4f61eeb2bdbad27316534016f");
	}

	TEST_CASE("sha-224") {
		sha2_engine engine (sha2_engine::SHA_224);

		engine.update ("abc");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "23097d223405d8228642a477bda255b32aadbce4bda0b3f7e36c9da7");

		engine.update ("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "75388b16512776cc5dba5da1fd890150b0c6455cb4f58b1952522525");

		engine
			.update ("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "c97ca9a559850ce97a04a96def6d99a9e0e0e2ab14e6b8df265fc0b3");

		for (int i = 0; i < 1000000; ++i) {
			engine.update ('a');
		}
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "20794655980c91d8bbb4c1ea97618a4bf03f42581948b2ee4ee7ad67");
	}

	TEST_CASE("sha-256") {
		sha2_engine engine (sha2_engine::SHA_256);

		engine.update ("abc");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");

		engine.update ("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1");

		engine
			.update ("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "cf5b16a778af8380036ce59e7b0492370b249b11e8f07a51afac45037afee9d1");

		for (int i = 0; i < 1000000; ++i) {
			engine.update ('a');
		}
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "cdc76e5c9914fb9281a1c7e284d73e67f1809a48a497200e046d39ccc7112cd0");
	}

	TEST_CASE("sha-384") {
		sha2_engine engine (sha2_engine::SHA_384);

		engine.update ("abc");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "cb00753f45a35e8bb5a03d699ac65007272c32ab0eded1631a8b605a43ff5bed8086072ba1e7cc2358baeca134c825a7");

		engine.update ("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "3391fdddfc8dc7393707a65b1b4709397cf8b1d162af05abfe8f450de5f36bc6b0455a8520bc4e6f5fe95b1fe3c8452b");

		engine
			.update ("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "09330c33f71147e83d192fc782cd1b4753111b173b3b05d22fa08086e3b0f712fcc7c71a557e2db966c3e9fa91746039");

		for (int i = 0; i < 1000000; ++i) {
			engine.update ('a');
		}
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "9d0e1809716474cb086e834e310a4a1ced149e9c00f248527972cec5704c2a5b07b8b3dc38ecc4ebae97ddd87f3d8985");
	}

	TEST_CASE("sha-512") {
		sha2_engine engine (sha2_engine::SHA_512);

		engine.update ("abc");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f");

		engine.update ("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "204a8fc6dda82f0a0ced7beb8e08a41657c16ef468b228a8279be331a703c33596fd15c13b1b07f9aa1d3bea57789ca031ad85c7a71dd70354ec631238ca3445");

		engine
			.update ("abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu");
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "8e959b75dae313da8cf4f72814fc143f8f7779c6eb9f7fa17299aeadb6889018501d289e4900f7e4331b99dec4b5433ac7d329eeb6dd26545e96e55b874be909");

		for (int i = 0; i < 1000000; ++i) {
			engine.update ('a');
		}
		REQUIRE (digest_engine::digest_to_hex (engine.digest ())
				 == "e718483d0ce769644e2e42c7bc15b4638e1f98b13b2044285632a803afa973ebde0ff244877ea60a4cb0432ce577c31beb009c5c2c49aa2e4eadb217ad8cc09b");

	}
}