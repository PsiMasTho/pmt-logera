file(GLOB TEST_SOURCES "*.cpp")
add_executable(${PROJECT_NAME}-unit-tests ${TEST_SOURCES})

target_include_directories(${PROJECT_NAME}-unit-tests PUBLIC
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)

target_link_libraries(${PROJECT_NAME}-unit-tests ${PROJECT_NAME})
