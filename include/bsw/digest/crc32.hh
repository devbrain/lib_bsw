//
// Created by igor on 2/29/24.
//

#ifndef BSW_INCLUDE_BSW_DIGEST_CRC32_HH_
#define BSW_INCLUDE_BSW_DIGEST_CRC32_HH_

#include <bsw/digest/digest_engine.hh>
#include <cstdint>

namespace bsw {

	class BSW_EXPORT crc32_engine : public digest_engine
		/// This class implements the MD4 message digest algorithm,
		/// described in RFC 1320.
	{
	 public:
		enum {
			DIGEST_SIZE = 4
		};

		crc32_engine ();
		~crc32_engine () override;

		crc32_engine (const crc32_engine&) = delete;
		crc32_engine& operator= (const crc32_engine&) = delete;

		[[nodiscard]] std::size_t digest_length () const override;
		void reset () override;
		const digest_t& digest () override;

	 protected:
		void update_impl (const void* data, std::size_t length) override;

	 private:
		void init();


		uint32_t m_context;
		digest_t m_digest;


	};

} // namespace Poco


#endif //BSW_INCLUDE_BSW_DIGEST_CRC32_HH_
