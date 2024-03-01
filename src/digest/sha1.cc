//
// Created by igor on 2/29/24.
//

#include <bsw/digest/sha1.hh>
#include <cstring>
#include <bsw/bsw_config.h>

#if defined(BSW_IS_LITTLE_ENDIAN)
#define SHA1_BYTE_REVERSE(x, y) byte_reverse(x, y)
#else
#define SHA1_BYTE_REVERSE(x, y)
#endif

namespace bsw {
	sha1_engine::sha1_engine () {
		m_digest.reserve (16);
		init();
	}

	sha1_engine::~sha1_engine () = default;

	inline void sha1_engine::byte_reverse ([[maybe_unused]] uint32_t* buffer, int byteCount) {
#if defined(BSW_IS_LITTLE_ENDIAN)
		byteCount /= sizeof (uint32_t);
		for (int count = 0; count < byteCount; count++) {
			uint32_t value = (buffer[count] << 16) | (buffer[count] >> 16);
			buffer[count] = ((value & 0xFF00FF00L) >> 8) | ((value & 0x00FF00FFL) << 8);
		}
#endif // BSW_IS_LITTLE_ENDIAN
	}

	void sha1_engine::update_impl (const void* buffer_, std::size_t count) {
		const byte_t* buffer = (const byte_t*)buffer_;
		byte_t* db = (byte_t*)&m_context.data[0];

		/* Update bitcount */
		if ((m_context.countLo + ((uint32_t)count << 3)) < m_context.countLo) {
			m_context.countHi++;
		} /* Carry from low to high bitCount */
		m_context.countLo += ((uint32_t)count << 3);
		m_context.countHi += ((uint32_t)count >> 29);

		/* Process data in BLOCK_SIZE chunks */
		while (count-- > 0) {
			db[m_context.slop++] = *(buffer++);
			if (m_context.slop == BLOCK_SIZE) {
				/* transform this one block */
				SHA1_BYTE_REVERSE(m_context.data, BLOCK_SIZE);
				transform ();
				m_context.slop = 0;    /* no slop left */
			}
		}
	}

	std::size_t sha1_engine::digest_length () const {
		return DIGEST_SIZE;
	}

	void sha1_engine::reset () {
		init();
	}

	void sha1_engine::init () {
		m_context.digest[0] = 0x67452301L;
		m_context.digest[1] = 0xEFCDAB89L;
		m_context.digest[2] = 0x98BADCFEL;
		m_context.digest[3] = 0x10325476L;
		m_context.digest[4] = 0xC3D2E1F0L;
		m_context.countLo = 0;
		m_context.countHi = 0;
		m_context.slop = 0;
		std::memset (m_context.data, 0, sizeof (m_context.data));
	}

	const digest_engine::digest_t& sha1_engine::digest () {
		int count;
		uint32_t lowBitcount = m_context.countLo;
		uint32_t highBitcount = m_context.countHi;

		/* Compute number of bytes mod 64 */
		count = (int)((m_context.countLo >> 3) & 0x3F);

		/* Set the first char of padding to 0x80.  This is safe since there is
			always at least one byte free */
		((byte_t*)m_context.data)[count++] = 0x80;

		/* Pad out to 56 mod 64 */
		if (count > 56) {
			/* Two lots of padding:  Pad the first block to 64 bytes */
			std::memset ((byte_t*)&m_context.data + count, 0, 64 - count);
			SHA1_BYTE_REVERSE(m_context.data, BLOCK_SIZE);
			transform ();

			/* Now fill the next block with 56 bytes */
			std::memset (&m_context.data, 0, 56);
		} else {
			/* Pad block to 56 bytes */
			std::memset ((byte_t*)&m_context.data + count, 0, 56 - count);
		}
		SHA1_BYTE_REVERSE(m_context.data, BLOCK_SIZE);

		/* Append length in bits and transform */
		m_context.data[14] = highBitcount;
		m_context.data[15] = lowBitcount;

		transform ();
		SHA1_BYTE_REVERSE(m_context.data, DIGEST_SIZE);

		unsigned char hash[DIGEST_SIZE];
		for (count = 0; count < DIGEST_SIZE; count++) {
			hash[count] = (byte_t) ((m_context.digest[count >> 2]) >> (8 * (3 - (count & 0x3)))) & 0xff;
		}
		m_digest.clear ();
		m_digest.insert (m_digest.begin (), hash, hash + DIGEST_SIZE);
		reset ();
		return m_digest;
	}


/* The SHA f()-functions */
#define f1(x, y, z)   ( ( x & y ) | ( ~x & z ) )              /* Rounds  0-19 */
#define f2(x, y, z)   ( x ^ y ^ z )                           /* Rounds 20-39 */
#define f3(x, y, z)   ( ( x & y ) | ( x & z ) | ( y & z ) )   /* Rounds 40-59 */
#define f4(x, y, z)   ( x ^ y ^ z )                           /* Rounds 60-79 */


/* The SHA Mysterious Constants */
#define K1  0x5A827999L     /* Rounds  0-19 */
#define K2  0x6ED9EBA1L     /* Rounds 20-39 */
#define K3  0x8F1BBCDCL     /* Rounds 40-59 */
#define K4  0xCA62C1D6L     /* Rounds 60-79 */

/* 32-bit rotate - kludged with shifts */
	typedef uint32_t UL;    /* to save space */


#define S(n, X)  ( ( ((UL)X) << n ) | ( ((UL)X) >> ( 32 - n ) ) )


/* The initial expanding function */
#define expand(count)   W[ count ] = S(1,(W[ count - 3 ] ^ W[ count - 8 ] ^ W[ count - 14 ] ^ W[ count - 16 ]))    /* to make this SHA-1 */


/* The four SHA sub-rounds */
#define subRound1(count)    \
{ \
    temp = S( 5, A ) + f1( B, C, D ) + E + W[ count ] + K1; \
    E = D; \
    D = C; \
    C = S( 30, B ); \
    B = A; \
    A = temp; \
}

#define subRound2(count)    \
{ \
    temp = S( 5, A ) + f2( B, C, D ) + E + W[ count ] + K2; \
    E = D; \
    D = C; \
    C = S( 30, B ); \
    B = A; \
    A = temp; \
}

#define subRound3(count)    \
{ \
    temp = S( 5, A ) + f3( B, C, D ) + E + W[ count ] + K3; \
    E = D; \
    D = C; \
    C = S( 30, B ); \
    B = A; \
    A = temp; \
}

#define subRound4(count)    \
{ \
    temp = S( 5, A ) + f4( B, C, D ) + E + W[ count ] + K4; \
    E = D; \
    D = C; \
    C = S( 30, B ); \
    B = A; \
    A = temp; \
}

	void sha1_engine::transform () {
		uint32_t W[80];
		uint32_t temp;
		uint32_t A, B, C, D, E;
		int i;

		/* Step A.  Copy the data buffer into the local work buffer */
		for (i = 0; i < 16; i++) {
			W[i] = m_context.data[i];
		}

		/* Step B.  Expand the 16 words into 64 temporary data words */
		expand(16);
		expand(17);
		expand(18);
		expand(19);
		expand(20);
		expand(21);
		expand(22);
		expand(23);
		expand(24);
		expand(25);
		expand(26);
		expand(27);
		expand(28);
		expand(29);
		expand(30);
		expand(31);
		expand(32);
		expand(33);
		expand(34);
		expand(35);
		expand(36);
		expand(37);
		expand(38);
		expand(39);
		expand(40);
		expand(41);
		expand(42);
		expand(43);
		expand(44);
		expand(45);
		expand(46);
		expand(47);
		expand(48);
		expand(49);
		expand(50);
		expand(51);
		expand(52);
		expand(53);
		expand(54);
		expand(55);
		expand(56);
		expand(57);
		expand(58);
		expand(59);
		expand(60);
		expand(61);
		expand(62);
		expand(63);
		expand(64);
		expand(65);
		expand(66);
		expand(67);
		expand(68);
		expand(69);
		expand(70);
		expand(71);
		expand(72);
		expand(73);
		expand(74);
		expand(75);
		expand(76);
		expand(77);
		expand(78);
		expand(79);

		/* Step C.  Set up first buffer */
		A = m_context.digest[0];
		B = m_context.digest[1];
		C = m_context.digest[2];
		D = m_context.digest[3];
		E = m_context.digest[4];

		/* Step D.  Serious mangling, divided into four sub-rounds */
		subRound1(0);
		subRound1(1);
		subRound1(2);
		subRound1(3);
		subRound1(4);
		subRound1(5);
		subRound1(6);
		subRound1(7);
		subRound1(8);
		subRound1(9);
		subRound1(10);
		subRound1(11);
		subRound1(12);
		subRound1(13);
		subRound1(14);
		subRound1(15);
		subRound1(16);
		subRound1(17);
		subRound1(18);
		subRound1(19);
		subRound2(20);
		subRound2(21);
		subRound2(22);
		subRound2(23);
		subRound2(24);
		subRound2(25);
		subRound2(26);
		subRound2(27);
		subRound2(28);
		subRound2(29);
		subRound2(30);
		subRound2(31);
		subRound2(32);
		subRound2(33);
		subRound2(34);
		subRound2(35);
		subRound2(36);
		subRound2(37);
		subRound2(38);
		subRound2(39);
		subRound3(40);
		subRound3(41);
		subRound3(42);
		subRound3(43);
		subRound3(44);
		subRound3(45);
		subRound3(46);
		subRound3(47);
		subRound3(48);
		subRound3(49);
		subRound3(50);
		subRound3(51);
		subRound3(52);
		subRound3(53);
		subRound3(54);
		subRound3(55);
		subRound3(56);
		subRound3(57);
		subRound3(58);
		subRound3(59);
		subRound4(60);
		subRound4(61);
		subRound4(62);
		subRound4(63);
		subRound4(64);
		subRound4(65);
		subRound4(66);
		subRound4(67);
		subRound4(68);
		subRound4(69);
		subRound4(70);
		subRound4(71);
		subRound4(72);
		subRound4(73);
		subRound4(74);
		subRound4(75);
		subRound4(76);
		subRound4(77);
		subRound4(78);
		subRound4(79);

		/* Step E.  Build message digest */
		m_context.digest[0] += A;
		m_context.digest[1] += B;
		m_context.digest[2] += C;
		m_context.digest[3] += D;
		m_context.digest[4] += E;
	}

}