#project(bzip2
#        VERSION 1.0.7
#        DESCRIPTION "This Bzip2/libbz2 a program and library for lossless block-sorting data compression."
#        LANGUAGES C)


set(LT_CURRENT 1)
set(LT_REVISION 7)
set(LT_AGE 0)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
include(Version)
include(SymLink)

set(BZ_VERSION 1.0.7)
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/bz_version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/bz_version.h
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

math(EXPR LT_SOVERSION "${LT_CURRENT} - ${LT_AGE}")
set(LT_VERSION "${LT_SOVERSION}.${LT_AGE}.${LT_REVISION}")
set(PACKAGE_VERSION ${PROJECT_VERSION})




# Support the latest c++ standard available.
include(ExtractValidFlags)

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

set(WARNCFLAGS)
if (CMAKE_C_COMPILER_ID MATCHES "MSVC")
    if (ENABLE_WERROR)
        set(WARNCFLAGS /WX)
    endif ()
else ()
    if (ENABLE_WERROR)
        extract_valid_c_flags(WARNCFLAGS -Werror)
    endif ()

    # For C compiler
    # Please keep this list in sync with meson.build
    extract_valid_c_flags(WARNCFLAGS

            -Wshadow

            )
endif ()


set(BZ2_SOURCES
        blocksort.c
        huffman.c
        crctable.c
        randtable.c
        compress.c
        decompress.c
        bzlib.c)

# The bz2 OBJECT-library, required for bzip2, bzip2recover.
add_library(bz2_ObjLib OBJECT)
set_target_properties(bz2_ObjLib PROPERTIES FOLDER ThirdParty)
target_sources(bz2_ObjLib
        PRIVATE ${BZ2_SOURCES}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/bzlib_private.h
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/bzlib.h)



