add_executable(unittest
        main.cc

        bsw/test_introspection.cc
        bsw/test_type_list.cc
        bsw/test_one_of.cc
        bsw/test_constexpr_for.cc
        bsw/test_type_name.cc
        bsw/strings/string_constant_test.cc
        bsw/strings/string_tokenizer_test.cc
        bsw/strings/string_utils_test.cc
        bsw/fpe_test.cc
        bsw/strings/test_numeric_string.cc
        bsw/strings/number_formatter_test.cc
        bsw/io/base_64_test.cc
        bsw/io/binary_reader_writer.cc
        bsw/io/memory_stream.cc
        bsw/io/zlib_test.cc
        bsw/io/tee_stream_test.cc
        bsw/io/stream_copy_test.cc
        bsw/test_id.cc
        bsw/digest/test_digest.cc
        bsw/io/digest_stream_test.cc
        bsw/test_uuid.cc
        bsw/test_random.cc
        bsw/uri_test.cc
        bsw/s11n/test_s11n.cc
        )

target_link_libraries(unittest
        PRIVATE
        bsw
        thirdparty::doctest
        )

target_include_directories(unittest
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_CONFIG_INCLUDE}
        )