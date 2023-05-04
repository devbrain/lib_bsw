cmake_minimum_required(VERSION 3.24)
project(bsw VERSION 1.0.0)

if (NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
    NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_C_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif()

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
option(bsw_INCLUDE_PACKAGING "Include packaging rules for bsw" "${is_top_level}")
option(bsw_ENABLE_TESTING "Enable testing for bsw" TRUE)

set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR};${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)

set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})
set(PROJECT_INCLUDE_PRIVATE ${CMAKE_SOURCE_DIR}/src)
set(PROJECT_INCLUDE ${CMAKE_SOURCE_DIR}/include)
set(PROJECT_CONFIG_INCLUDE ${CMAKE_BINARY_DIR}/include)


add_subdirectory(src)
if (bsw_ENABLE_TESTING)
    add_subdirectory(test)
    add_subdirectory(ext)
endif()


if (bsw_INCLUDE_PACKAGING)
    add_subdirectory(packaging)
endif ()

