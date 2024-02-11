add_executable(print-ast print_ast.cpp)
target_link_libraries(print-ast PRIVATE logera)

add_subdirectory(unit)
