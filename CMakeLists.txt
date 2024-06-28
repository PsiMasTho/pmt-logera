cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

if(NOT DEFINED PROJECT_NAME)
    set(LOGERA_IS_TOP_LEVEL ON)
else()
    set(LOGERA_IS_TOP_LEVEL OFF)
endif()

project(logera VERSION 1.0.0 LANGUAGES CXX)

option(LOGERA_ENABLE_LTO "Enable link-time optimization" OFF)
option(LOGERA_BUILD_TESTS "Build logera test program" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(LOGERA_BUILD_TO_CSV "Build logera to csv converter" ON)
option(LOGERA_BUILD_DUMP_AST "Build logera ast dumper" ON)
option(LOGERA_BUILD_FORMAT "Build logera formatter" ON)
option(LOGERA_INSTALL "Generate the install target" OFF)

if(LOGERA_ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT result OUTPUT output)
    if(result)
        message(STATUS "IPO/LTO enabled")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(WARNING "IPO/LTO not supported: ${output}")
    endif()
endif()

add_subdirectory(logera)

if(LOGERA_BUILD_TESTS AND LOGERA_IS_TOP_LEVEL)
    add_subdirectory(test)
endif()

if(LOGERA_BUILD_TO_CSV)
    add_subdirectory(logera_to_csv)
endif()

if(LOGERA_BUILD_DUMP_AST)
    add_subdirectory(logera_dump_ast)
endif()

if(LOGERA_BUILD_FORMAT)
    add_subdirectory(logera_format)
endif()
