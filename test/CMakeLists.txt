find_package(fmt REQUIRED)

add_executable(print-log-tokens print_log_tokens.cc)
target_compile_features(print-log-tokens PRIVATE cxx_std_20)
target_link_libraries(print-log-tokens PRIVATE logera fmt::fmt)

add_executable(print-log-tokens print_log_ast.cc)
target_compile_features(print-log-tokens PRIVATE cxx_std_20)
target_link_libraries(print-log-tokens PRIVATE logera fmt::fmt)

add_executable(print-header-tokens print_header_tokens.cc)
target_compile_features(print-header-tokens PRIVATE cxx_std_20)
target_link_libraries(print-header-tokens PRIVATE logera fmt::fmt)

add_subdirectory(unit)
