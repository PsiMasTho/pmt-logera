# Get a list of all .cpp files in the current directory
file(GLOB TEST_SOURCES "*.cpp")

# Loop through each source file
foreach(TEST_SOURCE ${TEST_SOURCES})
    # Extract the filename without the extension to use as the test name
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    # Create an executable for each test source file
    add_executable(${TEST_NAME} ${TEST_SOURCE})

    # Include directories for the test executable
    target_include_directories(${TEST_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

    # Link the main project library with the test executable
    target_link_libraries(${TEST_NAME} ${PROJECT_NAME})
endforeach()