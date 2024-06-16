include(FetchContent)
macro(set_target_folder tgt name)
    set_target_properties(${tgt} PROPERTIES FOLDER ${name})
endmacro()

macro(set_thirdparty tgt)
    set_target_folder(${tgt} "ThirdParty")
endmacro()

set(FETCHCONTENT_QUIET off)
#get_filename_component(fc_base "../fetched_content" REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
#set(FETCHCONTENT_BASE_DIR ${fc_base})

if (bsw_ENABLE_TESTING)
    add_subdirectory(doctest)
endif()

add_subdirectory(cpptrace)


