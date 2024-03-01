//
// Created by igor on 2/29/24.
//

#ifndef BSW_INCLUDE_BSW_DIGEST_MD4_HH_
#define BSW_INCLUDE_BSW_DIGEST_MD4_HH_

#include <bsw/digest/digest_engine.hh>
#include <cstdint>

namespace bsw {

	class BSW_EXPORT md4_engine : public digest_engine
		/// This class implements the MD4 message digest algorithm,
		/// described in RFC 1320.
	{
	 public:
		enum {
			BLOCK_SIZE = 64,
			DIGEST_SIZE = 16
		};

		md4_engine ();
		~md4_engine () override;

		[[nodiscard]] std::size_t digest_length () const override;
		void reset () override;
		const digest_t& digest () override;

	 protected:
		void update_impl (const void* data, std::size_t length) override;

	 private:
		void init();
		static void transform (uint32_t state[4], const unsigned char block[64]);
		static void encode (unsigned char* output, const uint32_t* input, std::size_t len);
		static void decode (uint32_t* output, const unsigned char* input, std::size_t len);

		struct context {
			uint32_t state[4];          // state (ABCD)
			uint32_t count[2];          // number of bits, modulo 2^64 (lsb first)
			unsigned char buffer[64]; // input buffer
		};

		context m_context;
		digest_t m_digest;

		md4_engine (const md4_engine&);
		md4_engine& operator= (const md4_engine&);
	};

} // namespace Poco


#endif //BSW_INCLUDE_BSW_DIGEST_MD4_HH_
