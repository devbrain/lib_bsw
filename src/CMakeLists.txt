include(GenerateExportHeader)

include(cmake/liblzma.cmake)
include(cmake/bz2.cmake)
include(cmake/zlib.cmake)
include(cmake/zstd.cmake)
include(cmake/brotli.cmake)

set(warn_hdr
        ${PROJECT_INCLUDE}/bsw/warn/pop.hh
        ${PROJECT_INCLUDE}/bsw/warn/push.hh
        )

function(GENERATE_WARN INTERNAL_NAME)
    cmake_parse_arguments(
            PARSED_ARGS # prefix of output variables
            "" # list of names of the boolean arguments (only defined ones will be true)
            "GCC;CLANG;MSVC;GCC_VER;CLANG_VER;MSVC_VER" # list of names of mono-valued arguments
            "" # list of names of multi-valued arguments (output variables are lists)
            ${ARGN} # arguments of the function to parse, here we take the all original ones
    )
    set (GCC_SUPPORT 0)
    if (PARSED_ARGS_GCC)
        set (GCC_INTERNAL_NAME "GCC_${INTERNAL_NAME}")
        if (PARSED_ARGS_GCC_VER)
            set (GCC_INTERNAL_VER ${PARSED_ARGS_GCC_VER})
        else()
            set (GCC_INTERNAL_VER 0)
        endif()
        set (GCC_SUPPORT 1)
    endif()

    set (CLANG_SUPPORT 0)
    if (PARSED_ARGS_CLANG)
        set (CLANG_INTERNAL_NAME "CLANG_${INTERNAL_NAME}")
        if (PARSED_ARGS_CLANG_VER)
            set (CLANG_INTERNAL_VER ${PARSED_ARGS_CLANG_VER})
        else()
            set (CLANG_INTERNAL_VER 0)
        endif()
        set (CLANG_SUPPORT 1)
    endif()

    set (MSVC_SUPPORT 0)
    if (PARSED_ARGS_MSVC)
        set (MSVC_INTERNAL_NAME "MSVC_${INTERNAL_NAME}")
        if (PARSED_ARGS_MSVC_VER)
            set (MSVC_INTERNAL_VER ${PARSED_ARGS_MSVC_VER})
        else ()
            set (MSVC_INTERNAL_VER 0)
        endif()
        set (MSVC_SUPPORT 1)
    endif()
    set (GENERATED_FILE  ${PROJECT_INCLUDE}/bsw/warn/${INTERNAL_NAME} )
    if(NOT EXISTS "${PROJECT_INCLUDE}/bsw/warn/${INTERNAL_NAME}")
        message("GENERATING  ${GENERATED_FILE}" )
        configure_file( ${PROJECT_INCLUDE}/bsw/warn/warn_config.h.in ${GENERATED_FILE} @ONLY)
    ENDIF()
    list(APPEND warn_hdr ${GENERATED_FILE})
    set (warn_hdr  ${warn_hdr} PARENT_SCOPE)
endfunction()
    GENERATE_WARN("undef" GCC "-Wundef"  CLANG "-Wundef")
    GENERATE_WARN("unused_macros" CLANG "-Wunused-macros")
    GENERATE_WARN("sign_conversion" CLANG "-Wsign-conversion" GCC "-Wsign-conversion")
    GENERATE_WARN("conv64to32" CLANG "-Wshorten-64-to-32" MSVC 4307)
    GENERATE_WARN("undefined_inline" CLANG "-Wundefined-inline")
    GENERATE_WARN("undefined_internal" CLANG "-Wundefined-internal")
    GENERATE_WARN("unneeded_internal_declaration" CLANG "-Wunneeded-internal-declaration")
    GENERATE_WARN("unneeded_member_function" CLANG "-Wunneeded-member-function")
    GENERATE_WARN("reserved_macro" CLANG "-Wreserved-id-macro")
    GENERATE_WARN("dll_interface" MSVC 4251)
    GENERATE_WARN("dll_interface_base" MSVC 4275)
    GENERATE_WARN("possible_loss_of_data" MSVC 4267)
    GENERATE_WARN("possible_loss_of_data_op" MSVC 4244)
    GENERATE_WARN("unreachable_code" MSVC 4702)
    GENERATE_WARN("define_deprecated" MSVC 4996)



# SOURCES ------------------------------------------------------------------------
set (src
         ${PROJECT_INCLUDE}/bsw/export.h

        ${PROJECT_INCLUDE}/bsw/mp/callable/helpers.hpp
        ${PROJECT_INCLUDE}/bsw/mp/callable/callable.hpp
        ${PROJECT_INCLUDE}/bsw/mp/callable/function.hpp
        ${PROJECT_INCLUDE}/bsw/mp/callable/member_function.hpp
        ${PROJECT_INCLUDE}/bsw/mp/callable/functor.hpp

        ${PROJECT_INCLUDE}/bsw/mp/primitive.hh
        ${PROJECT_INCLUDE}/bsw/mp/arithmetic_traits.hh

        ${PROJECT_INCLUDE}/bsw/mp/type_name/type_name.msvc.hpp
        ${PROJECT_INCLUDE}/bsw/mp/type_name/type_name.gcc.7_3.hpp
        ${PROJECT_INCLUDE}/bsw/mp/type_name/type_name.hpp
        ${PROJECT_INCLUDE}/bsw/mp/type_name/type_name.msvc.19_21.hpp
        ${PROJECT_INCLUDE}/bsw/mp/type_name/type_name.gcc.8_1.hpp
        ${PROJECT_INCLUDE}/bsw/mp/type_name/type_name.clang.5.hpp
        ${PROJECT_INCLUDE}/bsw/mp/type_name/type_name.clang.hpp
        ${PROJECT_INCLUDE}/bsw/mp/type_name/type_name.gcc.hpp

        ${PROJECT_INCLUDE}/bsw/mp/typelist.hh
        ${PROJECT_INCLUDE}/bsw/mp/if_then_else.hh
        ${PROJECT_INCLUDE}/bsw/mp/all_same.hh
        ${PROJECT_INCLUDE}/bsw/mp/one_of.hh
        ${PROJECT_INCLUDE}/bsw/mp/constexpr_for.hh
        ${PROJECT_INCLUDE}/bsw/mp/introspection.hh

        ${PROJECT_INCLUDE}/bsw/config_path.hh
        config_path.cc

        ${PROJECT_INCLUDE}/bsw/exception.hh
        exception.cc
        ${PROJECT_INCLUDE}/bsw/register_at_exit.hh
        register_at_exit.cc
        ${PROJECT_INCLUDE}/bsw/singleton.hh
        ${PROJECT_INCLUDE}/bsw/array_view.hh
        ${PROJECT_INCLUDE}/bsw/sorted_array.hh
        ${PROJECT_INCLUDE}/bsw/strong_type.hpp

        io/base64.cc
        ${PROJECT_INCLUDE}/bsw/io/base64.hh
        io/binary_reader.cc
        ${PROJECT_INCLUDE}/bsw/io/binary_reader.hh
        io/binary_writer.cc
        ${PROJECT_INCLUDE}/bsw/io/binary_writer.hh
        ${PROJECT_INCLUDE}/bsw/io/ios_init.hh
        io/memory_stream_buf.cc
        ${PROJECT_INCLUDE}/bsw/io/memory_stream_buf.hh
        ${PROJECT_INCLUDE}/bsw/io/buffered_stream_buf.hh
        ${PROJECT_INCLUDE}/bsw/io/buffered_bidi_stream_buf.hh
        ${PROJECT_INCLUDE}/bsw/io/unbuffered_stream_buf.hh
        ${PROJECT_INCLUDE}/bsw/io/deflating_stream.hh
        io/deflating_stream.cc
        ${PROJECT_INCLUDE}/bsw/io/inflating_stream.hh
        io/inflating_stream.cc
        ${PROJECT_INCLUDE}/bsw/io/compression_type.hh
        ${PROJECT_INCLUDE}/bsw/io/stream_copier.hh
        io/stream_copier.cc
        ${PROJECT_INCLUDE}/bsw/io/tee_stream.hh
        io/tee_stream.cc
        io/compression_stream/compression_stream.cc
        ${PROJECT_INCLUDE}/bsw/io/compression_stream/compression_stream.hh
        io/compression_stream/zstd_wrapper.h
        io/compression_stream/zstd_wrapper.c
        io/compression_stream/libz_stream.cc
        ${PROJECT_INCLUDE}/bsw/io/compression_stream/libz_stream.hh
        io/compression_stream/bzip_stream.cc
        ${PROJECT_INCLUDE}/bsw/io/compression_stream/bzip_stream.hh
        io/compression_stream/lzma_stream.cc
        ${PROJECT_INCLUDE}/bsw/io/compression_stream/lzma_stream.hh
        io/compression_stream/brotli_stream.cc
        ${PROJECT_INCLUDE}/bsw/io/compression_stream/brotli_stream.hh

        strings/ascii.cc
        ${PROJECT_INCLUDE}/bsw/strings/ascii.hh

        strings/number_parser.cc
        ${PROJECT_INCLUDE}/bsw/strings/number_parser.hh

        strings/number_formatter.cc
        ${PROJECT_INCLUDE}/bsw/strings/number_formatter.hh

        strings/numeric_string.cc
        ${PROJECT_INCLUDE}/bsw/strings/numeric_string.hh

        strings/string_tokenizer.cc
        ${PROJECT_INCLUDE}/bsw/strings/string_tokenizer.hh
        ${PROJECT_INCLUDE}/bsw/strings/string_hash.hh

        ${PROJECT_INCLUDE}/bsw/strings/wchar.hh
        strings/wchar.cc

        ${PROJECT_INCLUDE}/bsw/strings/string_constant.hh
        ${PROJECT_INCLUDE}/bsw/strings/string_utils.hh

        fpe/fpe.cc
        fpe/double-conversion/bignum.cc
        fpe/double-conversion/bignum-dtoa.cc
        fpe/double-conversion/bignum-dtoa.h
        fpe/double-conversion/bignum.h
        fpe/double-conversion/cached-powers.cc
        fpe/double-conversion/cached-powers.h
        fpe/double-conversion/diy-fp.h
        fpe/double-conversion/double-conversion.h
        fpe/double-conversion/double-to-string.cc
        fpe/double-conversion/double-to-string.h
        fpe/double-conversion/fast-dtoa.cc
        fpe/double-conversion/fast-dtoa.h
        fpe/double-conversion/fixed-dtoa.cc
        fpe/double-conversion/fixed-dtoa.h
        fpe/double-conversion/ieee.h
        fpe/double-conversion/string-to-double.cc
        fpe/double-conversion/string-to-double.h
        fpe/double-conversion/strtod.cc
        fpe/double-conversion/strtod.h
        fpe/double-conversion/utils.h
        ${PROJECT_INCLUDE}/bsw/fpe/fpe.hh
        ${PROJECT_INCLUDE}/bsw/fpe/fpe_c99.hh
        ${PROJECT_INCLUDE}/bsw/fpe/fpe_windows.hh

        console.cc
        ${PROJECT_INCLUDE}/bsw/console.hh
        whereami.cc
        ${PROJECT_INCLUDE}/bsw/whereami.hh

        thirdparty/whereami.c
        thirdparty/whereami.h
        thirdparty/inih.c
        thirdparty/inih.h

        ${PROJECT_INCLUDE}/bsw/byte_order.hh
        ${PROJECT_INCLUDE}/bsw/override.hh
        ${PROJECT_INCLUDE}/bsw/dlfcn.hh
        dlfcn.cc

        ${PROJECT_INCLUDE}/bsw/spimpl.h
        ${PROJECT_INCLUDE}/bsw/switch_by_string.hh

        ${PROJECT_INCLUDE}/bsw/observer.hh

        ${PROJECT_INCLUDE}/bsw/bit_ops.hh
        ${PROJECT_INCLUDE}/bsw/errors.hh
        errors.cc

        thirdparty/utf8.h
        thirdparty/utf8/checked.h
        thirdparty/utf8/core.h
        thirdparty/utf8/cpp11.h
        thirdparty/utf8/cpp17.h
        thirdparty/utf8/unchecked.h

        ${PROJECT_INCLUDE}/bsw/id.hh
        ${PROJECT_INCLUDE}/bsw/builtin.h

        logger/abstract_printer.cc
        logger/console.cc
        logger/console_printer.cc
        logger/debug_device.cc
        logger/device.cc
        logger/file_device.cc
        logger/formatter.cc
        logger/layout.cc
        logger/layout_commands.cc
        logger/level.cc
        logger/local_device.cc
        logger/locus.cc
        logger/logger_helper.cc
        logger/log_record_layout.cc
        logger/predicate.cc
        logger/record.cc
        logger/rotating_device.cc
        logger/simple_printer.cc
        logger/standard_layout.cc
        logger/system.cc
        ${PROJECT_INCLUDE}/bsw/logger/abstract_printer.hh
        ${PROJECT_INCLUDE}/bsw/logger/console.hh
        ${PROJECT_INCLUDE}/bsw/logger/console_printer.hh
        ${PROJECT_INCLUDE}/bsw/logger/debug_device.hh
        ${PROJECT_INCLUDE}/bsw/logger/device.hh
        ${PROJECT_INCLUDE}/bsw/logger/file_device.hh
        ${PROJECT_INCLUDE}/bsw/logger/formatter.hh
        ${PROJECT_INCLUDE}/bsw/logger/function_name.hh
        ${PROJECT_INCLUDE}/bsw/logger/layout_commands.hh
        ${PROJECT_INCLUDE}/bsw/logger/layout.hh
        ${PROJECT_INCLUDE}/bsw/logger/level.hh
        ${PROJECT_INCLUDE}/bsw/logger/local_device.hh
        ${PROJECT_INCLUDE}/bsw/logger/locus.hh
        ${PROJECT_INCLUDE}/bsw/logger/logger_fwd.hh
        ${PROJECT_INCLUDE}/bsw/logger/logger_helper.hh
        ${PROJECT_INCLUDE}/bsw/logger/logger.hh
        ${PROJECT_INCLUDE}/bsw/logger/log_record_layout.hh
        ${PROJECT_INCLUDE}/bsw/logger/multi_setter.hh
        ${PROJECT_INCLUDE}/bsw/logger/predicate.hh
        ${PROJECT_INCLUDE}/bsw/logger/record.hh
        ${PROJECT_INCLUDE}/bsw/logger/rotating_device.hh
        ${PROJECT_INCLUDE}/bsw/logger/simple_printer.hh
        ${PROJECT_INCLUDE}/bsw/logger/standard_layout.hh
        ${PROJECT_INCLUDE}/bsw/logger/system.hh

        mt/thread_id.cc
        mt/active_object.cc
        ${PROJECT_INCLUDE}/bsw/mt/active_object.hh
        ${PROJECT_INCLUDE}/bsw/mt/thread_id.hh

        ${PROJECT_INCLUDE}/bsw/digest/digest_engine.hh
        digest/digest_engine.cc
        ${PROJECT_INCLUDE}/bsw/digest/md4.hh
        digest/md4.cc
        ${PROJECT_INCLUDE}/bsw/digest/md5.hh
        digest/md5.cc
        ${PROJECT_INCLUDE}/bsw/digest/sha1.hh
        digest/sha1.cc
        ${PROJECT_INCLUDE}/bsw/digest/sha2.hh
        digest/sha2.cc
        ${PROJECT_INCLUDE}/bsw/io/digest_stream.hh
        io/digest_stream.cc
        ${PROJECT_INCLUDE}/bsw/digest/crc32.hh
        digest/crc32.cc

        ${PROJECT_INCLUDE}/bsw/uuid.hh
        uuid.cc

        ${PROJECT_INCLUDE}/bsw/uri.hh
        uri.cc

        ${PROJECT_INCLUDE}/bsw/random.hh
        random.cc
        ${PROJECT_INCLUDE}/bsw/io/random_stream.hh
        io/random_stream.cc

        ${PROJECT_INCLUDE}/bsw/environment.hh
        environment.cc

        ${PROJECT_INCLUDE}/bsw/shm.hh
        shm.cc

        ${PROJECT_INCLUDE}/bsw/uuid_generator.hh
        uuid_generator.cc

        ${PROJECT_INCLUDE}/bsw/macros.hh
        ${PROJECT_INCLUDE}/bsw/macros/preprocessor_for_each.h
        ${PROJECT_INCLUDE}/bsw/macros/ppcat.hh
        ${PROJECT_INCLUDE}/bsw/macros/stringize.hh
        ${PROJECT_INCLUDE}/bsw/macros/pp_narg.hh

        ${PROJECT_INCLUDE}/bsw/s11n/s11n.hh
        ${PROJECT_INCLUDE}/bsw/s11n/detail/cista.hh
        ${PROJECT_INCLUDE}/bsw/s11n/detail/traits_s11n.hh
        ${PROJECT_INCLUDE}/bsw/s11n/detail/archive.hh
        ${PROJECT_INCLUDE}/bsw/s11n/detail/s11n_convert.hh
        s11n/yaml_archive_impl.hh
        s11n/json_archive_impl.hh
        s11n/archive.cc
        s11n/yaml_archive_impl.cc
        s11n/json_archive_impl.cc

        ${PROJECT_INCLUDE}/bsw/json/json.hh
        ${PROJECT_INCLUDE}/bsw/json/json_fwd.hh
        json/json.cc

)
# --------------------------------------------------------------------------------
# LIBRARY
# --------------------------------------------------------------------------------
include(TestBigEndian)
test_big_endian(IS_BIG_ENDIAN)
if (NOT IS_BIG_ENDIAN)
    set(BSW_IS_LITTLE_ENDIAN 1)
endif ()

file(MAKE_DIRECTORY ${PROJECT_CONFIG_INCLUDE}/bsw)
configure_file(configure.h.in ${PROJECT_CONFIG_INCLUDE}/bsw/bsw_config.h @ONLY)



add_library(bsw
        ${src}

        ${bz2_src}
        ${liblzma_src}
        ${libz_src}
        ${zstd_src}
        ${BROTLI_COMMON_SOURCES}
        ${BROTLI_ENC_SOURCES}
        ${BROTLI_DEC_SOURCES}
        )

LZMA_TUKLIB_PRESETS()

generate_export_header(bsw EXPORT_FILE_NAME ${PROJECT_CONFIG_INCLUDE}/bsw/export_defines.h)

if (MSVC)
    foreach(source_file IN LISTS bz2_src liblzma_src libz_src zstd_src BROTLI_COMMON_SOURCES BROTLI_ENC_SOURCES BROTLI_DEC_SOURCES)
        set_source_files_properties(${source_file} PROPERTIES COMPILE_FLAGS /W0)
    endforeach()
endif()

foreach(source_file IN LISTS src)
    if (MSVC)
       set_source_files_properties(${source_file} PROPERTIES COMPILE_FLAGS /W4)
    else()
       set_source_files_properties(${source_file} PROPERTIES COMPILE_FLAGS "-Wall -Wextra -pedantic -Werror")
    endif()    
endforeach()

target_compile_definitions(
        bsw
        PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:BSW_STATIC_DEFINE>"
        PRIVATE ${BROTLI_DEFS} _LARGEFILE64_SOURCE  _CRT_SECURE_NO_WARNINGS
        )


add_library(bsw::bsw ALIAS bsw)
set_target_properties(bsw PROPERTIES
        VERSION ${bsw_VERSION}
        SOVERSION ${bsw_VERSION_MAJOR})

target_include_directories(bsw
        PUBLIC
            "$<BUILD_INTERFACE:${PROJECT_INCLUDE}>"
            "$<BUILD_INTERFACE:${PROJECT_CONFIG_INCLUDE}>"
            "$<INSTALL_INTERFACE:include>"
        PRIVATE
            ${CMAKE_CURRENT_BINARY_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${BROTLI_INCLUDE_DIRS}

        )
target_compile_features(bsw PUBLIC cxx_std_17)
set (win32_libs )
if (WIN32) 
    set (win32_libs Iphlpapi.lib)
endif()

target_link_libraries(bsw
        PUBLIC
            ${lzma_system_libs}
            ${BROTLI_LIBRARIES}
            ${win32_libs}
            cpptrace::cpptrace
            Boost::pfr
            thirdparty::cista
            thirdparty::json
            thirdparty::yml
        )

# On windows copy cpptrace.dll to the same directory as the executable for your_target
if(WIN32)
    add_custom_command(
            TARGET bsw POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:cpptrace::cpptrace>
            $<TARGET_FILE_DIR:bsw>
    )
endif()
