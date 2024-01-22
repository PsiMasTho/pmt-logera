include(GNUInstallDirs)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(LOGERA_BUILD_TO_CSV "Build logera to csv converter" ON)
option(LOGERA_INSTALL "Generate the install target" OFF)

file(GLOB LIB_SOURCES "*.c")
add_library(${PROJECT_NAME} ${LIB_SOURCES})

# PCRE2
find_package(PCRE2 REQUIRED)

if (NOT PCRE2_FOUND)
        message(FATAL_ERROR "PCRE2 not found")
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC ${PCRE2_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PCRE2_INCLUDE_DIRS})

target_include_directories(${PROJECT_NAME} PUBLIC
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

target_include_directories(${PROJECT_NAME} PUBLIC
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external>)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

if (LOGERA_INSTALL AND LOGERA_IS_TOP_LEVEL)
        install(TARGETS ${PROJECT_NAME}
                EXPORT ${PROJECT_NAME}-config
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
                ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
                INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

        install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

        install(EXPORT ${PROJECT_NAME}-config
                FILE ${PROJECT_NAME}-config.cmake
                DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
endif()
