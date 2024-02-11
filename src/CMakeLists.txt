include(GNUInstallDirs)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(LOGERA_BUILD_TO_CSV "Build logera to csv converter" ON)
option(LOGERA_INSTALL "Generate the install target" OFF)

file(GLOB LIB_SOURCES "*.cpp")
add_library(${PROJECT_NAME} ${LIB_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
$<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

target_include_directories(${PROJECT_NAME} PUBLIC
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external>)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

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

if (LOGERA_BUILD_TO_CSV)
        add_subdirectory(to_csv)
endif()
