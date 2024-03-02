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
#include "bsw/digest/digest_engine.hh"

namespace bsw {
	class BSW_EXPORT UUIDGenerator
		/// This class implements a generator for Universal Unique Identifiers,
		/// as specified in Appendix A of the DCE 1.1 Remote Procedure
		/// Call Specification (http://www.opengroup.org/onlinepubs/9629399/),
		/// RFC 2518 (WebDAV), section 6.4.1 and the UUIDs and GUIDs internet
		/// draft by Leach/Salz from February, 1998
		/// (http://ftp.ics.uci.edu/pub/ietf/webdav/uuid-guid/draft-leach-uuids-guids-01.txt)
		{
			public:
			UUIDGenerator();
			/// Creates the UUIDGenerator.

			~UUIDGenerator();
			/// Destroys the UUIDGenerator.

			uuid create();
			/// Creates a new time-based UUID, using the MAC address of
			/// one of the system's ethernet adapters.
			///
			/// Throws a SystemException if no MAC address can be
			/// obtained.

			uuid createFromName(const uuid& nsid, const std::string& name);
			/// Creates a name-based UUID.

			uuid createFromName(const uuid& nsid, const std::string& name, digest_engine& de);
			/// Creates a name-based UUID, using the given digest engine.
			///
			/// Note: in order to create a standard-compliant UUID, the given DigestEngine
			/// must be either an instance of MD5Engine or SHA1Engine. The version field of
			/// the UUID will be set accordingly.

			uuid createFromName(const uuid& nsid, const std::string& name, digest_engine& de, uuid::version_t version);
			/// Creates a name-based UUID, using the given digest engine and version.

			uuid createRandom();
			/// Creates a random UUID.

			uuid createOne();
			/// Tries to create and return a time-based UUID (see create()), and,
			/// if that does not work due to the unavailability of a MAC address,
			/// creates and returns a random UUID (see createRandom()).
			///
			/// The UUID::version() method can be used to determine the actual kind of
			/// the UUID generated.

			void seed(uint32_t n);
			/// Seeds the internal pseudo random generator for time-based UUIDs with the given seed.

			void seed();
			/// Seeds the internal pseudo random generator used for time-based UUIDs
			/// with a random seed obtained from a RandomInputStream.

			static UUIDGenerator& defaultGenerator();
			/// Returns a reference to the default UUIDGenerator.

			protected:
			uint32_t timeStamp();
			void getNode();

			private:
			std::mutex          _mutex;
			random              _random;
			uint32_t            _lastTime;
			int                 _ticks;
			Environment::NodeId _node;
			bool                _haveNode;

			UUIDGenerator(const UUIDGenerator&);
			UUIDGenerator& operator = (const UUIDGenerator&);
		};

}

#endif //BSW_INCLUDE_BSW_UUID_GENERATOR_HH_
