cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 20)
project(logera VERSION 1.0.0 LANGUAGES CXX)

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
