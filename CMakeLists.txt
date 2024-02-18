cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT DEFINED PROJECT_NAME)
    set(LOGERA_IS_TOP_LEVEL ON)
else()
    set(LOGERA_IS_TOP_LEVEL OFF)
endif()

project(logera VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(lib)
add_subdirectory(src)

option(BUILD_TESTS "Build test programs" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(LOGERA_INSTALL "Generate the install target" OFF)

if(BUILD_TESTS AND LOGERA_IS_TOP_LEVEL)
    add_subdirectory(test)
endif()
