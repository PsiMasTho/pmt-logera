file(GLOB SOURCES "*.cpp")

add_executable(${PROJECT_NAME}-to-csv ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)

target_include_directories(${PROJECT_NAME}-to-csv PUBLIC
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external>)

target_link_libraries(${PROJECT_NAME}-to-csv ${PROJECT_NAME})

if (LOGERA_INSTALL AND LOGERA_IS_TOP_LEVEL)
    install(TARGETS ${PROJECT_NAME}-to-csv DESTINATION bin)
endif()