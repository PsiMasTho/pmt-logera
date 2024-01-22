cmake_minimum_required(VERSION 3.9)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(NOT DEFINED PROJECT_NAME)
    set(LOGERA_IS_TOP_LEVEL ON)
else()
    set(LOGERA_IS_TOP_LEVEL OFF)
endif()

project(logera VERSION 1.0.0 LANGUAGES C)

#add_subdirectory(src)
add_subdirectory(lib)

option(BUILD_TESTS "Build test programs" OFF)

if(BUILD_TESTS AND LOGERA_IS_TOP_LEVEL)
    add_subdirectory(test)
endif()
