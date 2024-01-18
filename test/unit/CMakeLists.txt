file(GLOB TEST_SOURCES "*.c")
add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests ${PROJECT_NAME})