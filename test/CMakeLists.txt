
add_executable(print-log-tokens print_log_tokens.cc)

target_compile_features(print-log-tokens PRIVATE cxx_std_20)
target_link_libraries(print-log-tokens PRIVATE logera)

add_subdirectory(unit)
