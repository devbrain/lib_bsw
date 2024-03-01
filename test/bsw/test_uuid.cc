//
// Created by igor on 3/1/24.
//
#include <doctest/doctest.h>
#include <bsw/uuid.hh>

using namespace bsw;

TEST_SUITE("UUID") {
	TEST_CASE("parse")
	{
		uuid uuid ("6ba7b810-9dad-11d1-80b4-00c04fd430c8");
		REQUIRE (uuid.to_string () == "6ba7b810-9dad-11d1-80b4-00c04fd430c8");

		uuid.parse ("6BA7B810-9DAD-11D1-80B4-00C04FD430C8");
		REQUIRE (uuid.to_string () == "6ba7b810-9dad-11d1-80b4-00c04fd430c8");

		uuid.parse ("6BA7B8109DAD11D180B400C04FD430C8");
		REQUIRE (uuid.to_string () == "6ba7b810-9dad-11d1-80b4-00c04fd430c8");

		REQUIRE_THROWS(uuid.parse ("6xA7B8109DAD11D180B400C04FD430C8"));

		REQUIRE_THROWS(uuid.parse ("6xa7b810-9dad-11d1-80b4-00c04fd430c8"));

		REQUIRE_THROWS(uuid.parse ("6ba7b810-xdad-11d1-80b4-00c04fd430c8"));

		REQUIRE_THROWS(uuid.parse ("6ba7b810-9dad-x1d1-80b4-00c04fd430c8"));

		REQUIRE_THROWS(uuid.parse ("6ba7b810-9dad-11d1-x0b4-00c04fd430c8"));

		REQUIRE_THROWS(uuid.parse ("6ba7b810-9dad-11d1-80b4-00x04fd430c8"));

	}

	TEST_CASE("buffer")
	{
		uuid uuid1 ("6ba7b810-9dad-11d1-80b4-00c04fd430c8");
		char buffer[16];
		uuid1.copy_to (buffer);
		uuid uuid2;
		uuid2.copy_from (buffer);
		REQUIRE (uuid2.to_string () == "6ba7b810-9dad-11d1-80b4-00c04fd430c8");
	}

	TEST_CASE("compare")
	{
		uuid null;
		REQUIRE (null.is_null ());
		REQUIRE (uuid::null ().is_null ());

		uuid uuid1 = null;
		uuid uuid2;
		REQUIRE (uuid1.is_null ());
		REQUIRE (uuid1 == null);
		REQUIRE (!(uuid1 != null));
		REQUIRE (uuid1 >= null);
		REQUIRE (uuid1 <= null);
		REQUIRE (!(uuid1 > null));
		REQUIRE (!(uuid1 < null));
		REQUIRE (uuid1.to_string () == "00000000-0000-0000-0000-000000000000");

		uuid1 = uuid::dns ();
		REQUIRE (!uuid1.is_null ());
		REQUIRE (uuid1 != null);
		REQUIRE (!(uuid1 == null));
		REQUIRE (uuid1 >= null);
		REQUIRE (!(uuid1 <= null));
		REQUIRE (uuid1 > null);
		REQUIRE (!(uuid1 < null));
		REQUIRE (uuid1.to_string () == "6ba7b810-9dad-11d1-80b4-00c04fd430c8");

		REQUIRE (null != uuid1);
		REQUIRE (!(null == uuid1));
		REQUIRE (!(null >= uuid1));
		REQUIRE (null <= uuid1);
		REQUIRE (!(null > uuid1));
		REQUIRE (null < uuid1);

		uuid2 = uuid1;
		REQUIRE (uuid2 == uuid1);
		REQUIRE (!(uuid2 != uuid1));
		REQUIRE (uuid2 >= uuid1);
		REQUIRE (uuid2 <= uuid1);
		REQUIRE (!(uuid2 > uuid1));
		REQUIRE (!(uuid2 < uuid1));
	}

	TEST_CASE("version")
	{
		uuid uuid ("db4fa7e9-9e62-4597-99e0-b1ec0b59800e");
		uuid::version_t v = uuid.version ();
		REQUIRE (v == uuid::UUID_RANDOM);

		uuid.parse ("6ba7b810-9dad-11d1-80b4-00c04fd430c8");
		v = uuid.version ();
		REQUIRE (v == uuid::UUID_TIME_BASED);

		uuid.parse ("d2ee4220-3625-11d9-9669-0800200c9a66");
		v = uuid.version ();
		REQUIRE (v == uuid::UUID_TIME_BASED);

		uuid.parse ("360d3652-4411-4786-bbe6-b9675b548559");
		v = uuid.version ();
		REQUIRE (v == uuid::UUID_RANDOM);
	}

	TEST_CASE("variant")
	{
		uuid uuid ("db4fa7e9-9e62-4597-99e0-b1ec0b59800e");
		int v = uuid.variant ();
		REQUIRE (v == 2);

		uuid.parse ("6ba7b810-9dad-11d1-80b4-00c04fd430c8");
		v = uuid.variant ();
		REQUIRE (v == 2);

		uuid.parse ("d2ee4220-3625-11d9-9669-0800200c9a66");
		v = uuid.variant ();
		REQUIRE (v == 2);

		uuid.parse ("360d3652-4411-4786-bbe6-b9675b548559");
		v = uuid.variant ();
		REQUIRE (v == 2);
	}

	TEST_CASE("swap")
	{
		uuid uuid1 ("db4fa7e9-9e62-4597-99e0-b1ec0b59800e");
		uuid uuid2 ("d2ee4220-3625-11d9-9669-0800200c9a66");
		uuid1.swap (uuid2);
		REQUIRE (uuid1.to_string () == "d2ee4220-3625-11d9-9669-0800200c9a66");
		REQUIRE (uuid2.to_string () == "db4fa7e9-9e62-4597-99e0-b1ec0b59800e");
	}

	TEST_CASE("try parse")
	{
		uuid uuid1;
		REQUIRE (uuid1.try_parse ("6BA7B810-9DAD-11D1-80B4-00C04FD430C8"));
		REQUIRE (uuid1.to_string () == "6ba7b810-9dad-11d1-80b4-00c04fd430c8");

		uuid notUuid;
		REQUIRE (!notUuid.try_parse ("not a uuid"));
		REQUIRE (notUuid.is_null ());
	}

}