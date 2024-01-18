file(GLOB LIB_SOURCES "*.c")

include(GNUInstallDirs)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(LOGERA_BUILD_TO_CSV "Build logera to csv converter" ON)
option(LOGERA_INSTALL "Generate the install target" OFF)

set(GENERATED_LEXER ${CMAKE_CURRENT_SOURCE_DIR}/lexer.c)
set_property(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/lexer.c PROPERTY GENERATED 1)
add_custom_command(
   OUTPUT  ${GENERATED_LEXER}
   COMMAND ${PYTHON_EXECUTABLE} ${PROJECT_SOURCE_DIR}/tools/run_ragel.py --input-file ${CMAKE_CURRENT_SOURCE_DIR}/lexer.rl --output-file ${CMAKE_CURRENT_SOURCE_DIR}/lexer.c
   DEPENDS ${PROJECT_SOURCE_DIR}/tools/run_ragel.py
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lexer.rl
   COMMENT "Running ragel"
)

add_library(${PROJECT_NAME} ${LIB_SOURCES} ${GENERATED_LEXER} ${GENERATED_PARSER})

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
