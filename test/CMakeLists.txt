
add_executable(logera-print-tokens print_tokens.cc)

target_compile_features(logera-print-tokens PRIVATE cxx_std_20)
target_link_libraries(logera-print-tokens PRIVATE logera)

add_subdirectory(unit)
