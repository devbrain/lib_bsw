//
// Created by igor on 3/2/24.
//

#ifndef BSW_INCLUDE_BSW_UUID_GENERATOR_HH_
#define BSW_INCLUDE_BSW_UUID_GENERATOR_HH_

#include <mutex>
#include <bsw/export.h>
#include <bsw/uuid.hh>
#include <bsw/environment.hh>
#include <bsw/random.hh>
#include <chrono>
#include "bsw/digest/digest_engine.hh"

namespace bsw {
	class BSW_EXPORT uuid_generator
		/// This class implements a generator for Universal Unique Identifiers,
		/// as specified in Appendix A of the DCE 1.1 Remote Procedure
		/// Call Specification (http://www.opengroup.org/onlinepubs/9629399/),
		/// RFC 2518 (WebDAV), section 6.4.1 and the UUIDs and GUIDs internet
		/// draft by Leach/Salz from February, 1998
		/// (http://ftp.ics.uci.edu/pub/ietf/webdav/uuid-guid/draft-leach-uuids-guids-01.txt)
	{
	 public:
		using time_point_t = std::chrono::steady_clock::time_point;
	 public:
		uuid_generator ();
		/// Creates the UUIDGenerator.

		~uuid_generator ();
		/// Destroys the UUIDGenerator.

		uuid create ();
		/// Creates a new time-based UUID, using the MAC address of
		/// one of the system's ethernet adapters.
		///
		/// Throws a SystemException if no MAC address can be
		/// obtained.

		static uuid create_from_name (const uuid& nsid, const std::string& name);
		/// Creates a name-based UUID.

		static uuid create_from_name (const uuid& nsid, const std::string& name, digest_engine& de);
		/// Creates a name-based UUID, using the given digest engine.
		///
		/// Note: in order to create a standard-compliant UUID, the given DigestEngine
		/// must be either an instance of MD5Engine or SHA1Engine. The version field of
		/// the UUID will be set accordingly.

		static uuid create_from_name (const uuid& nsid, const std::string& name, digest_engine& de, uuid::version_t version);
		/// Creates a name-based UUID, using the given digest engine and version.

		static uuid create_random ();
		/// Creates a random UUID.

		uuid create_one ();
		/// Tries to create and return a time-based UUID (see create()), and,
		/// if that does not work due to the unavailability of a MAC address,
		/// creates and returns a random UUID (see create_random()).
		///
		/// The UUID::version() method can be used to determine the actual kind of
		/// the UUID generated.

		void seed (uint32_t n);
		/// Seeds the internal pseudo random generator for time-based UUIDs with the given seed.

		void seed ();
		/// Seeds the internal pseudo random generator used for time-based UUIDs
		/// with a random seed obtained from a RandomInputStream.

		static uuid_generator& default_generator ();
		/// Returns a reference to the default UUIDGenerator.

	 protected:
		long time_stamp ();

	 private:
		std::mutex m_mutex;
		random m_random;
		time_point_t m_last_time;
		int m_ticks;
		Environment::NodeId m_node;
		bool m_have_node;

		uuid_generator (const uuid_generator&);
		uuid_generator& operator= (const uuid_generator&);
	};

}

#endif //BSW_INCLUDE_BSW_UUID_GENERATOR_HH_
