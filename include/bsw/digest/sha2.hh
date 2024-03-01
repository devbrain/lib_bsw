//
// Created by igor on 2/29/24.
//

#ifndef BSW_INCLUDE_BSW_DIGEST_SHA2_HH_
#define BSW_INCLUDE_BSW_DIGEST_SHA2_HH_

#include <bsw/digest/digest_engine.hh>

namespace bsw {
	class BSW_EXPORT sha2_engine : public digest_engine
		/// This class implements the SHA-2 message digest algorithm.
		/// (FIPS 180-4, see http://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)
	{
	 public:
		enum algorithm_t {
			SHA_224 = 224,
			SHA_256 = 256,
			SHA_384 = 384,
			SHA_512 = 512
		};

		explicit sha2_engine (algorithm_t algorithm = SHA_256);
		~sha2_engine () override;

		sha2_engine (const sha2_engine&) = delete;
		sha2_engine& operator= (const sha2_engine&) = delete;

		[[nodiscard]] std::size_t digest_length () const override;
		void reset () override;
		const digest_engine::digest_t& digest () override;

	 protected:
		void update_impl (const void* data, std::size_t length) override;

	 private:
		void init();

		void* m_context;
		algorithm_t m_algorithm;
		digest_engine::digest_t m_digest;

	};

}

#endif //BSW_INCLUDE_BSW_DIGEST_SHA2_HH_
