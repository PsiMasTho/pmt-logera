cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 20)
project(Logera VERSION 1.0.0 LANGUAGES CXX)

########################################################################
# Source files
########################################################################
file(GLOB_RECURSE SOURCES "src/*.cc")

add_executable(${PROJECT_NAME} ${SOURCES})

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
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb3 -fsanitize=address -Wall -Wextra -Wpedantic")
    endif()

elseif(CMAKE_BUILD_TYPE STREQUAL "DebugUBSAN")

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb3 -fsanitize=undefined -Wall -Wextra -Wpedantic")
    endif()

elseif(CMAKE_BUILD_TYPE STREQUAL "DebugGDB" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb3 -Wall -Wextra -Wpedantic")
    endif()

elseif(CMAKE_BUILD_TYPE STREQUAL "DebugLIBCPP")

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -ggdb3 -fanalyzer -D_GLIBCXX_DEBUG_PEDANTIC")
    endif()

endif()

message(STATUS "Build type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")

