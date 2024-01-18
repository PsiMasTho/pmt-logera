add_executable(print-tokens print_tokens.c)
target_link_libraries(print-tokens PRIVATE logera)

add_executable(print-ast print_ast.c)
target_link_libraries(print-ast PRIVATE logera)

add_subdirectory(unit)
