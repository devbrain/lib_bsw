# ZLIB ------------------------------------------------------------------------
include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckCSourceCompiles)

check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stddef.h HAVE_STDDEF_H)

set (libz_defines)

if (HAVE_SYS_TYPES_H)
    list(APPEND libz_defines HAVE_SYS_TYPES_H)
endif ()
if (HAVE_STDINT_H)
    list(APPEND libz_defines HAVE_STDINT_H)
endif ()
if (HAVE_STDDEF_H)
    list(APPEND libz_defines HAVE_STDDEF_H)
endif ()
check_type_size(off64_t OFF64_T)
if (HAVE_OFF64_T)
    list(APPEND libz_defines _LARGEFILE64_SOURCE=1)
endif ()
check_function_exists(fseeko HAVE_FSEEKO)
if (NOT HAVE_FSEEKO)
    list(APPEND libz_defines NO_FSEEKO)
endif ()


#
# Check for unistd.h
#
check_include_file(unistd.h Z_HAVE_UNISTD_H)

if (MSVC)
    list(APPEND libz_defines _CRT_SECURE_NO_DEPRECATE)
    list(APPEND libz_defines _CRT_NONSTDC_NO_DEPRECATE)
endif ()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/zlib/zconf.h.cmakein
        ${CMAKE_CURRENT_BINARY_DIR}/zconf.h @ONLY)
set(libz_src
        thirdparty/zlib/adler32.c
        thirdparty/zlib/compress.c
        thirdparty/zlib/crc32.c
        thirdparty/zlib/deflate.c
        thirdparty/zlib/gzclose.c
        thirdparty/zlib/gzlib.c
        thirdparty/zlib/gzread.c
        thirdparty/zlib/gzwrite.c
        thirdparty/zlib/inflate.c
        thirdparty/zlib/infback.c
        thirdparty/zlib/inftrees.c
        thirdparty/zlib/inffast.c
        thirdparty/zlib/trees.c
        thirdparty/zlib/uncompr.c
        thirdparty/zlib/zutil.c
        ${CMAKE_CURRENT_BINARY_DIR}/zconf.h
        thirdparty/zlib/zlib.h
        thirdparty/zlib/crc32.h
        thirdparty/zlib/deflate.h
        thirdparty/zlib/gzguts.h
        thirdparty/zlib/inffast.h
        thirdparty/zlib/inffixed.h
        thirdparty/zlib/inflate.h
        thirdparty/zlib/inftrees.h
        thirdparty/zlib/trees.h
        thirdparty/zlib/zutil.h
        )
foreach (s ${libz_src})
    set_source_files_properties(${s} PROPERTIES COMPILE_DEFINITIONS "${libz_defines}")
    if (MSVC)
        set_source_files_properties(${s} PROPERTIES COMPILE_OPTIONS /W0)
    else()
        set_source_files_properties(${s} PROPERTIES COMPILE_OPTIONS -w)
    endif()
endforeach ()