//
// Created by igor on 2/29/24.
//

#ifndef BSW_INCLUDE_BSW_DIGEST_SHA1_HH_
#define BSW_INCLUDE_BSW_DIGEST_SHA1_HH_

#include <bsw/digest/digest_engine.hh>
#include <cstdint>

namespace bsw {
	class BSW_EXPORT sha1_engine : public digest_engine
		/// This class implements the SHA-1 message digest algorithm.
		/// (FIPS 180-1, see http://www.itl.nist.gov/fipspubs/fip180-1.htm)
	{
	 public:
		static constexpr auto BLOCK_SIZE = 64;
		static constexpr auto DIGEST_SIZE = 20;

		sha1_engine ();
		~sha1_engine () override;

		sha1_engine (const sha1_engine&) = delete;
		sha1_engine& operator= (const sha1_engine&) = delete;

		[[nodiscard]] std::size_t digest_length () const override;
		void reset () override;
		const digest_t& digest () override;

	 protected:
		void update_impl (const void* data, std::size_t length) override;

	 private:
		void init();
		void transform ();
		static void byte_reverse (uint32_t* buffer, int byteCount);

		typedef uint8_t byte_t;

		struct context {
			uint32_t digest[5]; // Message digest
			uint32_t countLo;   // 64-bit bit count
			uint32_t countHi;
			uint32_t data[16];  // SHA data buffer
			uint32_t slop;      // # of bytes saved in data[]
		};

		context m_context;
		digest_engine::digest_t m_digest;

	};

}

#endif //BSW_INCLUDE_BSW_DIGEST_SHA1_HH_
