include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if (NOT DEFINED bsw_INSTALL_CMAKEDIR)
    set(bsw_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/bsw" CACHE STRING "Path to bsw CMake files")
endif ()

install(TARGETS bsw EXPORT bsw_Targets
        RUNTIME COMPONENT bsw_Runtime
        LIBRARY COMPONENT bsw_Runtime
        NAMELINK_COMPONENT bsw_Development
        ARCHIVE COMPONENT bsw_Development
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(DIRECTORY
            "${bsw_SOURCE_DIR}/include/"
            "${bsw_BINARY_DIR}/src/include/"
        TYPE INCLUDE
        COMPONENT bsw_Development)

if (BUILD_SHARED_LIBS)
    set(type shared)
else ()
    set(type static)
endif ()

install(EXPORT bsw_Targets
        DESTINATION "${bsw_INSTALL_CMAKEDIR}"
        NAMESPACE bsw::
        FILE bsw-${type}-targets.cmake
        COMPONENT bsw_Development)

write_basic_package_version_file(
    bswConfigVersion.cmake
    COMPATIBILITY SameMajorVersion)

install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/bswConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/bswConfigVersion.cmake"
        DESTINATION "${bsw_INSTALL_CMAKEDIR}"
        COMPONENT bsw_Development)

