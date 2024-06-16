include(FetchContent)
include(folders)

set(FETCHCONTENT_QUIET off)
#get_filename_component(fc_base "../fetched_content" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
#set(FETCHCONTENT_BASE_DIR ${fc_base})

if (bsw_ENABLE_TESTING)
    add_subdirectory(doctest)
endif()

add_subdirectory(cpptrace)


