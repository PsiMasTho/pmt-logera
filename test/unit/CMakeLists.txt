# Tests
find_package(Catch2 REQUIRED)

# All tests sources
file(GLOB_RECURSE TEST_SOURCES "*.cc")
add_executable(${PROJECT_NAME}-tests ${TEST_SOURCES})

# Using c++20
target_compile_features(${PROJECT_NAME}-tests PRIVATE cxx_std_20)

# Should be linked to the main library, as well as the Catch2 testing library
target_link_libraries(${PROJECT_NAME}-tests PRIVATE logera Catch2::Catch2WithMain)