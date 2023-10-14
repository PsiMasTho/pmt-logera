cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 20)
project(Logera VERSION 1.0.0 LANGUAGES CXX)

########################################################################
# Program binary
########################################################################
file(GLOB_RECURSE PROJ_SOURCES "src/*.cc")
list(FILTER PROJ_SOURCES EXCLUDE REGEX "^(.*src/main.cc)$")

add_executable(${PROJECT_NAME} "src/main.cc" ${PROJ_SOURCES})

########################################################################
# Test binary
########################################################################

option(BUILD_TESTS "Build test program" OFF)

if(BUILD_TESTS)
    file(GLOB_RECURSE TEST_SOURCES "test/*.cc")
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_executable(unit_tests ${TEST_SOURCES} ${PROJ_SOURCES})
    target_compile_definitions(unit_tests PUBLIC BOOST_TEST_DYN_LINK)
    target_link_libraries(unit_tests Boost::unit_test_framework)
endif(BUILD_TESTS)

########################################################################
# Compiler flags
########################################################################

# Set the build type with a default value that should not be modified by the user
set(CMAKE_BUILD_TYPE "Default" CACHE INTERNAL "Do not change.")

# Set the CACHE INTERNAL property to signal that CMAKE_BUILD_TYPE should not be modified
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY HELPSTRING "Do not modify. Pick the options you want manually.")

# Set a meaningless default value for the build type
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Default")

set(OPTIMIZATION_LEVEL "max" CACHE STRING "Optimization level")
set_property(CACHE OPTIMIZATION_LEVEL PROPERTY STRINGS "max" "medium" "min")
set(WARNING_LEVEL "high" CACHE STRING "Warning level")
set_property(CACHE WARNING_LEVEL PROPERTY STRINGS "low" "high")
set(SANITIZER "none" CACHE STRING "Sanitizer to use")
set_property(CACHE SANITIZER PROPERTY STRINGS "none" "thread" "address" "undefined")

option(WITH_DEBUG_SYMBOLS "Include debug symbols in the binary" OFF)
option(NATIVE_BUILD "Optimize binary for the current machine" OFF)
option(LINK_TIME_OPTIMIZATION "Enable link time optimization" OFF)
option(DEBUG_STL "Enable debug STL version" OFF)

    # Optimization level
if (OPTIMIZATION_LEVEL STREQUAL "max")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options("-Ofast")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options("/O3")
    endif()
elseif (OPTIMIZATION_LEVEL STREQUAL "medium")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options("-O2")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options("/O2")
    endif()
elseif (OPTIMIZATION_LEVEL STREQUAL "min")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options("-O0")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options("/Od")
    endif()
endif()

    # Warning levels
if (WARNING_LEVEL STREQUAL "low")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    endif()
elseif (WARNING_LEVEL STREQUAL "high")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
endif()

    # Sanitizers
if (SANITIZER STREQUAL "thread")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options("-fsanitize=thread")
    else()
        message(WARNING "Thread sanitizer not supported for this compiler.")
    endif()
elseif (SANITIZER STREQUAL "address")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options("-fsanitize=address")
    else()
        message(WARNING "Address sanitizer not supported for this compiler.")
    endif()
elseif (SANITIZER STREQUAL "undefined")
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options("-fsanitize=undefined")
    else()
        message(WARNING "Undefined behavior sanitizer not supported for this compiler.")
    endif()
endif()

    # Debug symbols
if (WITH_DEBUG_SYMBOLS)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options("-ggdb3")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options("/DEBUG") # (todo: msvc /Zi also?)
    endif()
else()
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options("-s")
    endif()
endif()

    # Native build
if (NATIVE_BUILD)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options("-march=native")
    else()
        message(WARNING "Native build not supported for this compiler.")
    endif()
endif()

    # LTO
if (LINK_TIME_OPTIMIZATION)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT isIpoSupported OUTPUT ipoError)
    if (isIpoSupported)
        set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
        message(STATUS "Link time optimization supported and enabled.")
    else()
        message(WARNING "Link time optimization not supported. <${error}>.")
    endif()
endif()

    # Debug STL
if (DEBUG_STL)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_compile_options("-D_GLIBCXX_DEBUG_PEDANTIC")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options("-D_LIBCPP_DEBUG=1")
    endif()
endif()

########################################################################
# Messages
########################################################################

message(STATUS "Tests:          ${BUILD_TESTS}")

# Print the compile options
get_directory_property(compile_options COMPILE_OPTIONS)
message(STATUS "Compile options: ${compile_options}")
