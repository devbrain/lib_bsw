include(CheckCSourceCompiles)
check_c_source_compiles(
        "#if defined(__EMSCRIPTEN__)
   int main() {return 0;}
   #endif"
        BROTLI_EMSCRIPTEN
)
if (BROTLI_EMSCRIPTEN)
    message("-- Compiler is EMSCRIPTEN")
else ()
    message("-- Compiler is not EMSCRIPTEN")
endif ()

# Reads macro from .h file; it is expected to be a single-line define.
function(read_macro PATH MACRO OUTPUT)
    file(STRINGS ${PATH} _line REGEX "^#define +${MACRO} +(.+)$")
    string(REGEX REPLACE "^#define +${MACRO} +(.+)$" "\\1" _val "${_line}")
    set(${OUTPUT} ${_val} PARENT_SCOPE)
endfunction(read_macro)

set(BROTLI_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/brotli")

# Version information
read_macro("${BROTLI_ROOT}/c/common/version.h" "BROTLI_VERSION_MAJOR" BROTLI_VERSION_MAJOR)
read_macro("${BROTLI_ROOT}/c/common/version.h" "BROTLI_VERSION_MINOR" BROTLI_VERSION_MINOR)
read_macro("${BROTLI_ROOT}/c/common/version.h" "BROTLI_VERSION_PATCH" BROTLI_VERSION_PATCH)
set(BROTLI_VERSION "${BROTLI_VERSION_MAJOR}.${BROTLI_VERSION_MINOR}.${BROTLI_VERSION_PATCH}")
mark_as_advanced(BROTLI_VERSION BROTLI_VERSION_MAJOR BROTLI_VERSION_MINOR BROTLI_VERSION_PATCH)

# ABI Version information
read_macro("${BROTLI_ROOT}/c/common/version.h" "BROTLI_ABI_CURRENT" BROTLI_ABI_CURRENT)
read_macro("${BROTLI_ROOT}/c/common/version.h" "BROTLI_ABI_REVISION" BROTLI_ABI_REVISION)
read_macro("${BROTLI_ROOT}/c/common/version.h" "BROTLI_ABI_AGE" BROTLI_ABI_AGE)
math(EXPR BROTLI_ABI_COMPATIBILITY "${BROTLI_ABI_CURRENT} - ${BROTLI_ABI_AGE}")
mark_as_advanced(BROTLI_ABI_CURRENT BROTLI_ABI_REVISION BROTLI_ABI_AGE BROTLI_ABI_COMPATIBILITY)

include(CheckLibraryExists)
set(LIBM_LIBRARY)
CHECK_LIBRARY_EXISTS(m log2 "" HAVE_LIB_M)
if (HAVE_LIB_M)
    set(LIBM_LIBRARY "m")
endif ()

set(BROTLI_INCLUDE_DIRS "${BROTLI_ROOT}/c/include")
mark_as_advanced(BROTLI_INCLUDE_DIRS)
set(BROTLI_LIBRARIES ${LIBM_LIBRARY})
mark_as_advanced(BROTLI_LIBRARIES)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(BROTLI_DEFS OS_LINUX)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
    set(BROTLI_DEFS OS_FREEBSD)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(BROTLI_DEFS OS_MACOSX)
endif ()

set(BROTLI_COMMON_SOURCES
        thirdparty/brotli/c/common/shared_dictionary_internal.h
        thirdparty/brotli/c/common/platform.c
        thirdparty/brotli/c/common/transform.c
        thirdparty/brotli/c/common/dictionary.bin
        thirdparty/brotli/c/common/constants.h
        thirdparty/brotli/c/common/transform.h
        thirdparty/brotli/c/common/constants.c
        thirdparty/brotli/c/common/context.h
        thirdparty/brotli/c/common/shared_dictionary.c
        thirdparty/brotli/c/common/dictionary.c
        thirdparty/brotli/c/common/dictionary.h
        thirdparty/brotli/c/common/dictionary.bin.br
        thirdparty/brotli/c/common/context.c
        thirdparty/brotli/c/common/version.h
        thirdparty/brotli/c/common/platform.h
)

set(BROTLI_DEC_SOURCES
        thirdparty/brotli/c/dec/huffman.h
        thirdparty/brotli/c/dec/state.c
        thirdparty/brotli/c/dec/bit_reader.c
        thirdparty/brotli/c/dec/bit_reader.h
        thirdparty/brotli/c/dec/state.h
        thirdparty/brotli/c/dec/decode.c
        thirdparty/brotli/c/dec/prefix.h
        thirdparty/brotli/c/dec/huffman.c
)

set(BROTLI_ENC_SOURCES
        thirdparty/brotli/c/enc/entropy_encode.h
        thirdparty/brotli/c/enc/backward_references_hq.c
        thirdparty/brotli/c/enc/fast_log.c
        thirdparty/brotli/c/enc/literal_cost.c
        thirdparty/brotli/c/enc/hash_composite_inc.h
        thirdparty/brotli/c/enc/bit_cost_inc.h
        thirdparty/brotli/c/enc/metablock_inc.h
        thirdparty/brotli/c/enc/ringbuffer.h
        thirdparty/brotli/c/enc/compound_dictionary.c
        thirdparty/brotli/c/enc/histogram.c
        thirdparty/brotli/c/enc/bit_cost.c
        thirdparty/brotli/c/enc/compress_fragment.c
        thirdparty/brotli/c/enc/compress_fragment.h
        thirdparty/brotli/c/enc/histogram.h
        thirdparty/brotli/c/enc/entropy_encode.c
        thirdparty/brotli/c/enc/memory.h
        thirdparty/brotli/c/enc/static_dict_lut.h
        thirdparty/brotli/c/enc/hash_to_binary_tree_inc.h
        thirdparty/brotli/c/enc/utf8_util.h
        thirdparty/brotli/c/enc/utf8_util.c
        thirdparty/brotli/c/enc/compress_fragment_two_pass.h
        thirdparty/brotli/c/enc/cluster_inc.h
        thirdparty/brotli/c/enc/hash_longest_match_inc.h
        thirdparty/brotli/c/enc/static_dict.h
        thirdparty/brotli/c/enc/block_splitter.c
        thirdparty/brotli/c/enc/state.h
        thirdparty/brotli/c/enc/block_splitter.h
        thirdparty/brotli/c/enc/backward_references.h
        thirdparty/brotli/c/enc/write_bits.h
        thirdparty/brotli/c/enc/hash.h
        thirdparty/brotli/c/enc/backward_references_inc.h
        thirdparty/brotli/c/enc/dictionary_hash.h
        thirdparty/brotli/c/enc/encoder_dict.h
        thirdparty/brotli/c/enc/entropy_encode_static.h
        thirdparty/brotli/c/enc/metablock.h
        thirdparty/brotli/c/enc/command.c
        thirdparty/brotli/c/enc/prefix.h
        thirdparty/brotli/c/enc/quality.h
        thirdparty/brotli/c/enc/command.h
        thirdparty/brotli/c/enc/metablock.c
        thirdparty/brotli/c/enc/block_encoder_inc.h
        thirdparty/brotli/c/enc/cluster.h
        thirdparty/brotli/c/enc/find_match_length.h
        thirdparty/brotli/c/enc/memory.c
        thirdparty/brotli/c/enc/hash_rolling_inc.h
        thirdparty/brotli/c/enc/cluster.c
        thirdparty/brotli/c/enc/compound_dictionary.h
        thirdparty/brotli/c/enc/brotli_bit_stream.c
        thirdparty/brotli/c/enc/params.h
        thirdparty/brotli/c/enc/histogram_inc.h
        thirdparty/brotli/c/enc/block_splitter_inc.h
        thirdparty/brotli/c/enc/literal_cost.h
        thirdparty/brotli/c/enc/encoder_dict.c
        thirdparty/brotli/c/enc/brotli_bit_stream.h
        thirdparty/brotli/c/enc/backward_references_hq.h
        thirdparty/brotli/c/enc/dictionary_hash.c
        thirdparty/brotli/c/enc/static_dict.c
        thirdparty/brotli/c/enc/hash_longest_match64_inc.h
        thirdparty/brotli/c/enc/hash_forgetful_chain_inc.h
        thirdparty/brotli/c/enc/compress_fragment_two_pass.c
        thirdparty/brotli/c/enc/bit_cost.h
        thirdparty/brotli/c/enc/hash_longest_match_quickly_inc.h
        thirdparty/brotli/c/enc/fast_log.h
        thirdparty/brotli/c/enc/backward_references.c
        thirdparty/brotli/c/enc/encode.c

)






