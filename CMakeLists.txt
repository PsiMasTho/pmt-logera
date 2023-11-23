cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 20)
project(logera VERSION 1.0.0 LANGUAGES CXX)


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fsanitize=address,undefined -fsanitize-undefined-trap-on-error -ggdb3 -O0 -D_LIBCPP_DEBUG=1")
set (CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address -fsanitize=undefined -fsanitize-undefined-trap-on-error")

########################################################################
# Library and executable
########################################################################

add_subdirectory(src)

########################################################################
# Tests
########################################################################

option(BUILD_TESTS "Build test program" OFF)

if(BUILD_TESTS)
    add_subdirectory(test)
endif(BUILD_TESTS)

########################################################################
# Messages
########################################################################

message(STATUS "Tests:          ${BUILD_TESTS}")
