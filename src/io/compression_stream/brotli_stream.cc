//
// Created by igor on 2/29/24.
//

#include <variant>
#include <bsw/exception.hh>
#include <brotli/encode.h>
#include <brotli/decode.h>
#include "bsw/io/compression_stream/brotli_stream.hh"

namespace bsw::io {
	namespace detail {
		struct brotli_impl {
			brotli_impl ()
				: available_in (0),
				  next_in (nullptr),
				  available_out (0),
				  next_out (nullptr) {

			}

			std::variant<std::monostate, BrotliEncoderState*, BrotliDecoderState*> state;
			size_t available_in;
			uint8_t* next_in;
			size_t available_out;
			uint8_t* next_out;
		};
	}

	brotli_stream::brotli_stream ()
		: m_pimpl (spimpl::make_unique_impl<detail::brotli_impl> ()) {

	}

	const uint8_t* brotli_stream::next_in () const {
		return m_pimpl->next_in;
	}

	std::size_t brotli_stream::avail_in () const {
		return m_pimpl->available_in;
	}

	uint8_t* brotli_stream::next_out () const {
		return m_pimpl->next_out;
	}

	std::size_t brotli_stream::avail_out () const {
		return m_pimpl->available_out;
	}

	void brotli_stream::set_next_in (const unsigned char* in) {
		m_pimpl->next_in = const_cast<unsigned char*>(in);
	}

	void brotli_stream::set_avail_in (std::size_t in) {
		m_pimpl->available_in = in;
	}

	void brotli_stream::set_next_out (const uint8_t* in) {
		m_pimpl->next_out = const_cast<unsigned char*>(in);
	}

	void brotli_stream::set_avail_out (std::size_t in) {
		m_pimpl->available_out = in;
	}

	brotli_stream::~brotli_stream () = default;

	brotli_compressor::brotli_compressor () {
		auto enc = BrotliEncoderCreateInstance (nullptr, nullptr, nullptr);
		ENFORCE(enc);
		m_pimpl->state = enc;
	}

	brotli_compressor::~brotli_compressor () {
		if (auto* enc = std::get_if<BrotliEncoderState*> (&m_pimpl->state)) {
			BrotliEncoderDestroyInstance (*enc);
		}
	}

	compression_stream_base::status_t brotli_compressor::compress (compression_stream_base::flush_mode_t flags) {
		auto* enc = std::get_if<BrotliEncoderState*> (&m_pimpl->state);
		ENFORCE(*enc);
		BrotliEncoderOperation ops = BROTLI_OPERATION_PROCESS;
		switch (flags) {
			case NO_FLUSH:
				ops = BROTLI_OPERATION_PROCESS;
				break;
			case PARTIAL_FLUSH:
			case SYNC_FLUSH:
			case FULL_FLUSH: ops = BROTLI_OPERATION_FLUSH;
				break;
			case FINISH: ops = BROTLI_OPERATION_FINISH;
				break;
		}
		auto rc = BrotliEncoderCompressStream (*enc,
											   ops,
											   &m_pimpl->available_in,
											   (const uint8_t**)(&m_pimpl->next_in),
											   &m_pimpl->available_out,
											   &m_pimpl->next_out, nullptr);
		if (rc) {
			if (BrotliEncoderIsFinished(*enc)) {
				return STREAM_END;
			}
			return compression_stream_base::OK;
		}
		return compression_stream_base::DATA_ERROR;
	}

	void brotli_compressor::finalize () {
		ENFORCE(compression_stream_base::OK == compress (FINISH));
	}

	brotli_decompressor::brotli_decompressor () {
		auto enc = BrotliDecoderCreateInstance (nullptr, nullptr, nullptr);
		ENFORCE(enc);
		m_pimpl->state = enc;
	}

	brotli_decompressor::~brotli_decompressor () {
		if (auto* enc = std::get_if<BrotliDecoderState*> (&m_pimpl->state)) {
			BrotliDecoderDestroyInstance (*enc);
		}
	}

	compression_stream_base::status_t brotli_decompressor::decompress ([[maybe_unused]] compression_stream_base::flush_mode_t flags) {
		auto* enc = std::get_if<BrotliDecoderState*> (&m_pimpl->state);
		ENFORCE(*enc);
		BrotliDecoderResult retval = BrotliDecoderDecompressStream (*enc,
																	&m_pimpl->available_in,
																	(const uint8_t**)(&m_pimpl->next_in),
																	&m_pimpl->available_out,
																	&m_pimpl->next_out, nullptr);
		if (BrotliDecoderIsFinished (*enc)) {
			return STREAM_END;
		}

		switch (retval) {
			case BROTLI_DECODER_RESULT_SUCCESS:
				return OK;
			case BROTLI_DECODER_RESULT_ERROR:
				return DATA_ERROR;
			case BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT:
			case BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT:
				return OK;
		}
		return compression_stream_base::DATA_ERROR;
	}

	void brotli_decompressor::finalize () {

	}

	void brotli_decompressor::reset () {

	}

}

