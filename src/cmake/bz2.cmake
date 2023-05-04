# BZIP2 --------------------------------------------------------------------------
set(BZ_VERSION 1.0.7)
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/bzip2/bz_version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/bz_version.h
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Support the latest c++ standard available.
# include(ExtractValidFlags)

# Checks for header files.
include(CheckIncludeFile)
check_include_file(arpa/inet.h HAVE_ARPA_INET_H)
check_include_file(fcntl.h HAVE_FCNTL_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(limits.h HAVE_LIMITS_H)
check_include_file(netdb.h HAVE_NETDB_H)
check_include_file(netinet/in.h HAVE_NETINET_IN_H)
check_include_file(pwd.h HAVE_PWD_H)
check_include_file(sys/socket.h HAVE_SYS_SOCKET_H)
check_include_file(sys/time.h HAVE_SYS_TIME_H)
check_include_file(syslog.h HAVE_SYSLOG_H)
check_include_file(time.h HAVE_TIME_H)
check_include_file(unistd.h HAVE_UNISTD_H)

include(CheckTypeSize)
# Checks for typedefs, structures, and compiler characteristics.
# AC_TYPE_SIZE_T
check_type_size("ssize_t" SIZEOF_SSIZE_T)
if (NOT SIZEOF_SSIZE_T)
    # ssize_t is a signed type in POSIX storing at least -1.
    # Set it to "int" to match the behavior of AC_TYPE_SSIZE_T (autotools).
    set(ssize_t int)
endif ()

include(CheckStructHasMember)
check_struct_has_member("struct tm" tm_gmtoff time.h HAVE_STRUCT_TM_TM_GMTOFF)

# Checks for library functions.
include(CheckFunctionExists)
check_function_exists(_Exit HAVE__EXIT)
check_function_exists(accept4 HAVE_ACCEPT4)
check_function_exists(mkostemp HAVE_MKOSTEMP)

include(CheckSymbolExists)
# XXX does this correctly detect initgroups (un)availability on cygwin?
check_symbol_exists(initgroups grp.h HAVE_DECL_INITGROUPS)
if (NOT HAVE_DECL_INITGROUPS AND HAVE_UNISTD_H)
    # FreeBSD declares initgroups() in unistd.h
    check_symbol_exists(initgroups unistd.h HAVE_DECL_INITGROUPS2)
    if (HAVE_DECL_INITGROUPS2)
        set(HAVE_DECL_INITGROUPS 1)
    endif ()
endif ()
set (bz2_src
        thirdparty/bzip2/blocksort.c
        thirdparty/bzip2/huffman.c
        thirdparty/bzip2/crctable.c
        thirdparty/bzip2/randtable.c
        thirdparty/bzip2/compress.c
        thirdparty/bzip2/decompress.c
        thirdparty/bzip2/bzlib.c
        )
foreach (s ${bz2_src})
    if (MSVC)
        set_source_files_properties(${s} PROPERTIES COMPILE_OPTIONS /W0)
    else()
        set_source_files_properties(${s} PROPERTIES COMPILE_OPTIONS -w)
    endif()
endforeach ()