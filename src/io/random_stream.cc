//
// Created by igor on 3/1/24.
//

#include <bsw/io/random_stream.hh>
#include <bsw/digest/sha1.hh>
#include <bsw/predef.h>
#include <bsw/io/ios_init.hh>
#include <bsw/random.hh>

#if defined(PREDEF_OS_WINDOWS)
#include <wincrypt.h>
#else
#include <fcntl.h>
#include <unistd.h>

#endif
#include <ctime>

namespace bsw::io {
	random_buf::random_buf ()
		: buffered_stream_buf (256, std::ios::in) {
	}

	random_buf::~random_buf () = default;

	int random_buf::read_from_device (char* buffer, std::streamsize length) {
		int n = 0;

#if defined(PREDEF_OS_WINDOWS)
	HCRYPTPROV hProvider = 0;
	CryptAcquireContext(&hProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
	CryptGenRandom(hProvider, (DWORD) length, (BYTE*) buffer);
	CryptReleaseContext(hProvider, 0);
	n = static_cast<int>(length);
#else
#if defined(PREDEF_PLATFORM_UNIX)
		int fd = open ("/dev/urandom", O_RDONLY, 0);
		if (fd >= 0) {
			n = read (fd, buffer, length);
			close (fd);
		}
#endif
		if (n <= 0) {
			// x is here as a source of randomness, so it does not make
			// much sense to protect it with a Mutex.
			static uint32_t x = 0;
			random rnd1 (256);
			random rnd2 (64);
			x += rnd1.next ();

			n = 0;
			sha1_engine engine;
			auto t = (uint32_t)std::time (nullptr);
			engine.update (&t, sizeof (t));
			void* p = this;
			engine.update (&p, sizeof (p));
			engine.update (buffer, length);
			uint32_t junk[32];
			engine.update (junk, sizeof (junk));
			while (n < length) {
				for (int i = 0; i < 100; ++i) {
					uint32_t r = rnd2.next ();
					engine.update (&r, sizeof (r));
					engine.update (&x, sizeof (x));
					x += rnd1.next ();
				}
				auto d = engine.digest ();
				for (auto it = d.begin (); it != d.end () && n < length; ++it, ++n) {
					engine.update (*it);
					*buffer++ = *it;
				}
			}
		}
#endif
		return n;
	}

	random_ios::random_ios () {
		bsw_ios_init(&_buf);
	}

	random_ios::~random_ios () = default;

	random_buf* random_ios::rdbuf () {
		return &_buf;
	}

	random_input_stream::random_input_stream ()
		: std::istream (&_buf) {
	}

	random_input_stream::~random_input_stream () = default;

}