file(GLOB SOURCES "*.cpp")

add_executable(${PROJECT_NAME}-dump-ast ${SOURCES})

target_include_directories(${PROJECT_NAME}-dump-ast PUBLIC
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)

target_link_libraries(${PROJECT_NAME}-dump-ast ${PROJECT_NAME})

if (LOGERA_INSTALL AND LOGERA_IS_TOP_LEVEL)
    install(TARGETS ${PROJECT_NAME}-dump-ast DESTINATION bin)
endif()