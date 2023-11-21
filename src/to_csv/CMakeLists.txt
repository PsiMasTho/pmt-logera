file(GLOB SOURCES "*.cc")

add_executable(${PROJECT_NAME}-to-csv ${SOURCES})
target_link_libraries(${PROJECT_NAME}-to-csv fmt::fmt ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}-to-csv DESTINATION bin)