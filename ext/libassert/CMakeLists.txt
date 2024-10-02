set(LIBASSERT_USE_EXTERNAL_CPPTRACE ON CACHE BOOL  "${LIBASSERT_USE_EXTERNAL_CPPTRACE}" FORCE)
set(LIBASSERT_USE_EXTERNAL_MAGIC_ENUM ON CACHE BOOL  "${LIBASSERT_USE_EXTERNAL_MAGIC_ENUM}" FORCE)

FetchContent_Declare(
        libassert
        GIT_REPOSITORY https://github.com/jeremy-rifkin/libassert.git
        GIT_TAG        v2.1.1 # <HASH or TAG>
)
FetchContent_MakeAvailable(libassert)
#target_link_libraries(your_target libassert::assert)
#
## On windows copy libassert.dll to the same directory as the executable for your_target
#if(WIN32)
#    add_custom_command(
#            TARGET your_target POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            $<TARGET_FILE:libassert::assert>
#            $<TARGET_FILE_DIR:your_target>
#    )
#endif()