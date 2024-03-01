//
// Created by igor on 2/29/24.
//

#ifndef BSW_INCLUDE_BSW_DIGEST_DIGEST_ENGINE_HH_
#define BSW_INCLUDE_BSW_DIGEST_DIGEST_ENGINE_HH_

#include <vector>
#include <string>
#include <bsw/export.h>

namespace bsw {

	class BSW_EXPORT digest_engine
		/// This class is an abstract base class
		/// for all classes implementing a message
		/// digest algorithm, like MD5Engine
		/// and SHA1Engine.
		/// Call update() repeatedly with data to
		/// compute the digest from. When done,
		/// call digest() to obtain the message
		/// digest.
	{
	 public:
		using digest_t = std::vector<unsigned char>;

		digest_engine ();
		virtual ~digest_engine ();

		digest_engine (const digest_engine&) = delete;
		digest_engine& operator= (const digest_engine&) = delete;

		void update (const void* data, std::size_t length);
		void update (char data);
		void update (const std::string& data);
		/// Updates the digest with the given data.

		virtual std::size_t digest_length () const = 0;
		/// Returns the length of the digest in bytes.

		virtual void reset () = 0;
		/// Resets the engine so that a new
		/// digest can be computed.

		virtual const digest_t& digest () = 0;
		/// Finishes the computation of the digest and
		/// returns the message digest. Resets the engine
		/// and can thus only be called once for every digest.
		/// The returned reference is valid until the next
		/// time digest() is called, or the engine object is destroyed.

		static std::string digest_to_hex (const digest_t& bytes);
		/// Converts a message digest into a string of hexadecimal numbers.

		static digest_t digest_from_hex (const std::string& digest);
		/// Converts a string created by digest_to_hex back to its Digest presentation

		static bool constant_time_equals (const digest_t& d1, const digest_t& d2);
		/// Compares two Digest values using a constant-time comparison
		/// algorithm. This can be used to prevent timing attacks
		/// (as discussed in <https://codahale.com/a-lesson-in-timing-attacks/>).

	 protected:
		virtual void update_impl (const void* data, std::size_t length) = 0;
		/// Updates the digest with the given data. Must be implemented
		/// by subclasses.

	 private:

	};


//
// inlines
//


	inline void digest_engine::update (const void* data, std::size_t length) {
		update_impl (data, length);
	}

	inline void digest_engine::update (char data) {
		update_impl (&data, 1);
	}

	inline void digest_engine::update (const std::string& data) {
		update_impl (data.data (), data.size ());
	}

} // namespace Poco
#endif //BSW_INCLUDE_BSW_DIGEST_DIGEST_ENGINE_HH_
