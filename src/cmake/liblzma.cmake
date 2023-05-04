# LZMA --------------------------------------------------------------------------
include(CheckSymbolExists)
include(CheckStructHasMember)
include(thirdparty/liblzma/cmake/tuklib_integer.cmake)
include(thirdparty/liblzma/cmake/tuklib_cpucores.cmake)
include(thirdparty/liblzma/cmake/tuklib_physmem.cmake)
include(thirdparty/liblzma/cmake/tuklib_progname.cmake)
include(thirdparty/liblzma/cmake/tuklib_mbstr.cmake)

file(READ thirdparty/liblzma/src/liblzma/api/lzma/version.h XZ_VERSION)
string(REGEX REPLACE
        "^.*\n\
#define LZMA_VERSION_MAJOR ([0-9]+)\n\
#define LZMA_VERSION_MINOR ([0-9]+)\n\
#define LZMA_VERSION_PATCH ([0-9]+)\n\
.*$"
        "\\1.\\2.\\3" XZ_VERSION "${XZ_VERSION}")

# _GNU_SOURCE and such definitions. This specific macro is special since
# it also adds the definitions to CMAKE_REQUIRED_DEFINITIONS.
tuklib_use_system_extensions(ALL)

# This is needed by liblzma and xz.
tuklib_integer(ALL)
set(lzma_system_libs )
set(liblzma_defines
        # Package info:
        PACKAGE_NAME="XZ Utils"
        PACKAGE_BUGREPORT="lasse.collin@tukaani.org"
        PACKAGE_URL="https://tukaani.org/xz/"

        # Features:
        HAVE_CHECK_CRC32
        HAVE_CHECK_CRC64
        HAVE_CHECK_SHA256
        HAVE_DECODERS
        HAVE_DECODER_ARM
        HAVE_DECODER_ARMTHUMB
        HAVE_DECODER_DELTA
        HAVE_DECODER_IA64
        HAVE_DECODER_LZMA1
        HAVE_DECODER_LZMA2
        HAVE_DECODER_POWERPC
        HAVE_DECODER_SPARC
        HAVE_DECODER_X86
        HAVE_ENCODERS
        HAVE_ENCODER_ARM
        HAVE_ENCODER_ARMTHUMB
        HAVE_ENCODER_DELTA
        HAVE_ENCODER_IA64
        HAVE_ENCODER_LZMA1
        HAVE_ENCODER_LZMA2
        HAVE_ENCODER_POWERPC
        HAVE_ENCODER_SPARC
        HAVE_ENCODER_X86
        HAVE_MF_BT2
        HAVE_MF_BT3
        HAVE_MF_BT4
        HAVE_MF_HC3
        HAVE_MF_HC4

        # Standard headers and types are available:
        HAVE_STDBOOL_H
        HAVE__BOOL
        HAVE_STDINT_H
        HAVE_INTTYPES_H
        TUKLIB_SYMBOL_PREFIX=lzma_
        NDEBUG
	  LZMA_API_STATIC	
        )
# Check for clock_gettime(). Do this before checking for threading so
# that we know there if CLOCK_MONOTONIC is available.
if (NOT WIN32 AND NOT DEFINED CACHE{HAVE_CLOCK_GETTIME})
    check_symbol_exists(clock_gettime time.h HAVE_CLOCK_GETTIME)
    if (NOT HAVE_CLOCK_GETTIME)
        # With glibc <= 2.17 or Solaris 10 this needs librt.
        unset(HAVE_CLOCK_GETTIME CACHE)

        list(INSERT lzma_system_libs 0 rt)
        check_symbol_exists(clock_gettime time.h HAVE_CLOCK_GETTIME)

        # If it was found now, add it to all targets and keep it
        # in CMAKE_REQUIRED_LIBRARIES for further tests too.
        if (NOT HAVE_CLOCK_GETTIME)
            list(REMOVE_AT lzma_system_libs 0)
        else ()

        endif ()
    endif ()
    if (HAVE_CLOCK_GETTIME)
        # Check if CLOCK_MONOTONIC is available for clock_gettime().
        check_symbol_exists(CLOCK_MONOTONIC time.h HAVE_DECL_CLOCK_MONOTONIC)

        # HAVE_DECL_CLOCK_MONOTONIC should always be defined to 0 or 1
        # when clock_gettime is available.
        list(INSERT liblzma_defines 0
                HAVE_CLOCK_GETTIME
                HAVE_DECL_CLOCK_MONOTONIC=$<BOOL:"${HAVE_DECL_CLOCK_MONOTONIC}">
                )
    endif ()
endif ()

# Threading support:
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
if (CMAKE_USE_WIN32_THREADS_INIT)
    list(INSERT liblzma_defines 0 MYTHREAD_VISTA)
else ()
    list(INSERT liblzma_defines 0 MYTHREAD_POSIX)
    # Check if pthread_condattr_setclock() exists to use CLOCK_MONOTONIC.
    if (HAVE_DECL_CLOCK_MONOTONIC)
        list(INSERT lzma_system_libs 0 "${CMAKE_THREAD_LIBS_INIT}")
        check_symbol_exists(pthread_condattr_setclock pthread.h
                HAVE_PTHREAD_CONDATTR_SETCLOCK)
        tuklib_add_definition_if(ALL HAVE_PTHREAD_CONDATTR_SETCLOCK)
    endif ()
endif ()



set (liblzma_src
        thirdparty/liblzma/src/common/mythread.h
        thirdparty/liblzma/src/common/sysdefs.h
        thirdparty/liblzma/src/common/tuklib_common.h
        thirdparty/liblzma/src/common/tuklib_config.h
        thirdparty/liblzma/src/common/tuklib_cpucores.c
        thirdparty/liblzma/src/common/tuklib_cpucores.h
        thirdparty/liblzma/src/common/tuklib_integer.h
        thirdparty/liblzma/src/common/tuklib_physmem.c
        thirdparty/liblzma/src/common/tuklib_physmem.h
        thirdparty/liblzma/src/liblzma/api/lzma.h
        thirdparty/liblzma/src/liblzma/api/lzma/base.h
        thirdparty/liblzma/src/liblzma/api/lzma/bcj.h
        thirdparty/liblzma/src/liblzma/api/lzma/block.h
        thirdparty/liblzma/src/liblzma/api/lzma/check.h
        thirdparty/liblzma/src/liblzma/api/lzma/container.h
        thirdparty/liblzma/src/liblzma/api/lzma/delta.h
        thirdparty/liblzma/src/liblzma/api/lzma/filter.h
        thirdparty/liblzma/src/liblzma/api/lzma/hardware.h
        thirdparty/liblzma/src/liblzma/api/lzma/index.h
        thirdparty/liblzma/src/liblzma/api/lzma/index_hash.h
        thirdparty/liblzma/src/liblzma/api/lzma/lzma12.h
        thirdparty/liblzma/src/liblzma/api/lzma/stream_flags.h
        thirdparty/liblzma/src/liblzma/api/lzma/version.h
        thirdparty/liblzma/src/liblzma/api/lzma/vli.h
        thirdparty/liblzma/src/liblzma/check/check.c
        thirdparty/liblzma/src/liblzma/check/check.h
        thirdparty/liblzma/src/liblzma/check/crc32_fast.c
        thirdparty/liblzma/src/liblzma/check/crc32_table.c
        thirdparty/liblzma/src/liblzma/check/crc32_table_be.h
        thirdparty/liblzma/src/liblzma/check/crc32_table_le.h
        thirdparty/liblzma/src/liblzma/check/crc64_fast.c
        thirdparty/liblzma/src/liblzma/check/crc64_table.c
        thirdparty/liblzma/src/liblzma/check/crc64_table_be.h
        thirdparty/liblzma/src/liblzma/check/crc64_table_le.h
        thirdparty/liblzma/src/liblzma/check/crc_macros.h
        thirdparty/liblzma/src/liblzma/check/sha256.c
        thirdparty/liblzma/src/liblzma/common/alone_decoder.c
        thirdparty/liblzma/src/liblzma/common/alone_decoder.h
        thirdparty/liblzma/src/liblzma/common/alone_encoder.c
        thirdparty/liblzma/src/liblzma/common/auto_decoder.c
        thirdparty/liblzma/src/liblzma/common/block_buffer_decoder.c
        thirdparty/liblzma/src/liblzma/common/block_buffer_encoder.c
        thirdparty/liblzma/src/liblzma/common/block_buffer_encoder.h
        thirdparty/liblzma/src/liblzma/common/block_decoder.c
        thirdparty/liblzma/src/liblzma/common/block_decoder.h
        thirdparty/liblzma/src/liblzma/common/block_encoder.c
        thirdparty/liblzma/src/liblzma/common/block_encoder.h
        thirdparty/liblzma/src/liblzma/common/block_header_decoder.c
        thirdparty/liblzma/src/liblzma/common/block_header_encoder.c
        thirdparty/liblzma/src/liblzma/common/block_util.c
        thirdparty/liblzma/src/liblzma/common/common.c
        thirdparty/liblzma/src/liblzma/common/common.h
        thirdparty/liblzma/src/liblzma/common/easy_buffer_encoder.c
        thirdparty/liblzma/src/liblzma/common/easy_decoder_memusage.c
        thirdparty/liblzma/src/liblzma/common/easy_encoder.c
        thirdparty/liblzma/src/liblzma/common/easy_encoder_memusage.c
        thirdparty/liblzma/src/liblzma/common/easy_preset.c
        thirdparty/liblzma/src/liblzma/common/easy_preset.h
        thirdparty/liblzma/src/liblzma/common/filter_buffer_decoder.c
        thirdparty/liblzma/src/liblzma/common/filter_buffer_encoder.c
        thirdparty/liblzma/src/liblzma/common/filter_common.c
        thirdparty/liblzma/src/liblzma/common/filter_common.h
        thirdparty/liblzma/src/liblzma/common/filter_decoder.c
        thirdparty/liblzma/src/liblzma/common/filter_decoder.h
        thirdparty/liblzma/src/liblzma/common/filter_encoder.c
        thirdparty/liblzma/src/liblzma/common/filter_encoder.h
        thirdparty/liblzma/src/liblzma/common/filter_flags_decoder.c
        thirdparty/liblzma/src/liblzma/common/filter_flags_encoder.c
        thirdparty/liblzma/src/liblzma/common/hardware_cputhreads.c
        thirdparty/liblzma/src/liblzma/common/hardware_physmem.c
        thirdparty/liblzma/src/liblzma/common/index.c
        thirdparty/liblzma/src/liblzma/common/index.h
        thirdparty/liblzma/src/liblzma/common/index_decoder.c
        thirdparty/liblzma/src/liblzma/common/index_encoder.c
        thirdparty/liblzma/src/liblzma/common/index_encoder.h
        thirdparty/liblzma/src/liblzma/common/index_hash.c
        thirdparty/liblzma/src/liblzma/common/memcmplen.h
        thirdparty/liblzma/src/liblzma/common/outqueue.c
        thirdparty/liblzma/src/liblzma/common/outqueue.h
        thirdparty/liblzma/src/liblzma/common/stream_buffer_decoder.c
        thirdparty/liblzma/src/liblzma/common/stream_buffer_encoder.c
        thirdparty/liblzma/src/liblzma/common/stream_decoder.c
        thirdparty/liblzma/src/liblzma/common/stream_decoder.h
        thirdparty/liblzma/src/liblzma/common/stream_encoder.c
        thirdparty/liblzma/src/liblzma/common/stream_encoder_mt.c
        thirdparty/liblzma/src/liblzma/common/stream_flags_common.c
        thirdparty/liblzma/src/liblzma/common/stream_flags_common.h
        thirdparty/liblzma/src/liblzma/common/stream_flags_decoder.c
        thirdparty/liblzma/src/liblzma/common/stream_flags_encoder.c
        thirdparty/liblzma/src/liblzma/common/vli_decoder.c
        thirdparty/liblzma/src/liblzma/common/vli_encoder.c
        thirdparty/liblzma/src/liblzma/common/vli_size.c
        thirdparty/liblzma/src/liblzma/delta/delta_common.c
        thirdparty/liblzma/src/liblzma/delta/delta_common.h
        thirdparty/liblzma/src/liblzma/delta/delta_decoder.c
        thirdparty/liblzma/src/liblzma/delta/delta_decoder.h
        thirdparty/liblzma/src/liblzma/delta/delta_encoder.c
        thirdparty/liblzma/src/liblzma/delta/delta_encoder.h
        thirdparty/liblzma/src/liblzma/delta/delta_private.h
        thirdparty/liblzma/src/liblzma/lz/lz_decoder.c
        thirdparty/liblzma/src/liblzma/lz/lz_decoder.h
        thirdparty/liblzma/src/liblzma/lz/lz_encoder.c
        thirdparty/liblzma/src/liblzma/lz/lz_encoder.h
        thirdparty/liblzma/src/liblzma/lz/lz_encoder_hash.h
        thirdparty/liblzma/src/liblzma/lz/lz_encoder_hash_table.h
        thirdparty/liblzma/src/liblzma/lz/lz_encoder_mf.c
        thirdparty/liblzma/src/liblzma/lzma/fastpos.h
        thirdparty/liblzma/src/liblzma/lzma/fastpos_table.c
        thirdparty/liblzma/src/liblzma/lzma/lzma2_decoder.c
        thirdparty/liblzma/src/liblzma/lzma/lzma2_decoder.h
        thirdparty/liblzma/src/liblzma/lzma/lzma2_encoder.c
        thirdparty/liblzma/src/liblzma/lzma/lzma2_encoder.h
        thirdparty/liblzma/src/liblzma/lzma/lzma_common.h
        thirdparty/liblzma/src/liblzma/lzma/lzma_decoder.c
        thirdparty/liblzma/src/liblzma/lzma/lzma_decoder.h
        thirdparty/liblzma/src/liblzma/lzma/lzma_encoder.c
        thirdparty/liblzma/src/liblzma/lzma/lzma_encoder.h
        thirdparty/liblzma/src/liblzma/lzma/lzma_encoder_optimum_fast.c
        thirdparty/liblzma/src/liblzma/lzma/lzma_encoder_optimum_normal.c
        thirdparty/liblzma/src/liblzma/lzma/lzma_encoder_presets.c
        thirdparty/liblzma/src/liblzma/lzma/lzma_encoder_private.h
        thirdparty/liblzma/src/liblzma/rangecoder/price.h
        thirdparty/liblzma/src/liblzma/rangecoder/price_table.c
        thirdparty/liblzma/src/liblzma/rangecoder/range_common.h
        thirdparty/liblzma/src/liblzma/rangecoder/range_decoder.h
        thirdparty/liblzma/src/liblzma/rangecoder/range_encoder.h
        thirdparty/liblzma/src/liblzma/simple/arm.c
        thirdparty/liblzma/src/liblzma/simple/armthumb.c
        thirdparty/liblzma/src/liblzma/simple/ia64.c
        thirdparty/liblzma/src/liblzma/simple/powerpc.c
        thirdparty/liblzma/src/liblzma/simple/simple_coder.c
        thirdparty/liblzma/src/liblzma/simple/simple_coder.h
        thirdparty/liblzma/src/liblzma/simple/simple_decoder.c
        thirdparty/liblzma/src/liblzma/simple/simple_decoder.h
        thirdparty/liblzma/src/liblzma/simple/simple_encoder.c
        thirdparty/liblzma/src/liblzma/simple/simple_encoder.h
        thirdparty/liblzma/src/liblzma/simple/simple_private.h
        thirdparty/liblzma/src/liblzma/simple/sparc.c
        thirdparty/liblzma/src/liblzma/simple/x86.c
        )
set(liblzma_includes
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/liblzma/src/liblzma/api
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/liblzma/src/liblzma/common
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/liblzma/src/liblzma/check
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/liblzma/src/liblzma/lz
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/liblzma/src/liblzma/rangecoder
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/liblzma/src/liblzma/lzma
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/liblzma/src/liblzma/delta
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/liblzma/src/liblzma/simple
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/liblzma/src/common
        )


foreach (s ${liblzma_src})
    set_source_files_properties(${s} PROPERTIES INCLUDE_DIRECTORIES "${liblzma_includes}")
    set_source_files_properties(${s} PROPERTIES COMPILE_DEFINITIONS "${liblzma_defines}")
endforeach ()

macro(LZMA_TUKLIB_PRESETS)
    tuklib_cpucores(bsw)
    tuklib_physmem(bsw)

    # While liblzma can be built without tuklib_cpucores or tuklib_physmem
    # modules, the liblzma API functions lzma_cputhreads() and lzma_physmem()
    # will then be useless (which isn't too bad but still unfortunate). Since
    # I expect the CMake-based builds to be only used on systems that are
    # supported by these tuklib modules, problems with these tuklib modules
    # are considered a hard error for now. This hopefully helps to catch bugs
    # in the CMake versions of the tuklib checks.
    if (NOT TUKLIB_CPUCORES_FOUND OR NOT TUKLIB_PHYSMEM_FOUND)
        # Use SEND_ERROR instead of FATAL_ERROR. If someone reports a bug,
        # seeing the results of the remaining checks can be useful too.
        message(SEND_ERROR
                "tuklib_cpucores() or tuklib_physmem() failed. "
                "Unless you really are building for a system where these "
                "modules are not supported (unlikely), this is a bug in the "
                "included cmake/tuklib_*.cmake files that should be fixed. "
                "To build anyway, edit this CMakeLists.txt to ignore this error.")
    endif ()

    # immintrin.h:
    include(CheckIncludeFile)
    check_include_file(immintrin.h HAVE_IMMINTRIN_H)
    if (HAVE_IMMINTRIN_H)
        target_compile_definitions(bsw PRIVATE HAVE_IMMINTRIN_H)

        # SSE2 intrinsics:
        include(CheckCSourceCompiles)
        check_c_source_compiles("
            #include <immintrin.h>
            int main(void)
            {
                __m128i x = { 0 };
                _mm_movemask_epi8(x);
                return 0;
            }
        "
                HAVE__MM_MOVEMASK_EPI8)
        tuklib_add_definition_if(bsw HAVE__MM_MOVEMASK_EPI8)
    endif ()
endmacro()