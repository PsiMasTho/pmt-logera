cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 20)
project(Logera VERSION 1.0.0 LANGUAGES CXX)

########################################################################
# Program binary
########################################################################
file(GLOB_RECURSE PROJ_SOURCES "src/*.cc")
list(REMOVE_ITEM PROJ_SOURCES "src/main.cc")

add_library(Proj STATIC ${PROJ_SOURCES})
add_executable(${PROJECT_NAME} "src/main.cc")
target_link_libraries(${PROJECT_NAME} Proj)

########################################################################
# Test binary
########################################################################

option(BUILD_TESTS "Build test program" OFF)

if(BUILD_TESTS)
    file(GLOB_RECURSE TEST_SOURCES "test/*.cc")
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_executable(unit_tests ${TEST_SOURCES})
    target_compile_definitions(unit_tests PUBLIC BOOST_TEST_DYN_LINK)
    target_link_libraries(unit_tests Proj Boost::unit_test_framework)
endif(BUILD_TESTS)

########################################################################
# Compiler flags
########################################################################

    #Default to release
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()


if (CMAKE_BUILD_TYPE STREQUAL "Release")

    include(CheckIPOSupported)
    check_ipo_supported(RESULT isIpoSupported OUTPUT ipoError)
    if (isIpoSupported)
        set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
        message(STATUS "Link time optimization supported and enabled.")
    else()
        message(WARNING "Link time optimization not supported. <${error}>.")
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -march=native -s")
        add_definitions(-DNDEBUG=1)
    endif()

elseif(CMAKE_BUILD_TYPE STREQUAL "DebugASAN")

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb3 -fsanitize=address -Wall -Wextra -Wpedantic -Wconversion")
    endif()

elseif(CMAKE_BUILD_TYPE STREQUAL "DebugUBSAN")

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb3 -fsanitize=undefined -Wall -Wextra -Wpedantic -Wconversion")
    endif()

elseif(CMAKE_BUILD_TYPE STREQUAL "DebugGDB")

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb3 -Wall -Wextra -Wpedantic -Wconversion")
    endif()

elseif(CMAKE_BUILD_TYPE STREQUAL "DebugCACHEGRIND")

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -march=native -ggdb3 -Wall -Wextra -Wpedantic -Wconversion")
    endif()

elseif(CMAKE_BUILD_TYPE STREQUAL "DebugLIBCPP")

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb3 -fanalyzer -D_GLIBCXX_DEBUG_PEDANTIC")
    endif()

else()
    message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif()

########################################################################
# Messages
########################################################################

message(STATUS "Build type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Tests:          ${BUILD_TESTS}")

