option(LOGERA_BUILD_TO_CSV "Build logera to csv converter" ON)

if(LOGERA_BUILD_TO_CSV)
    add_subdirectory(to_csv)
endif()

option(LOGERA_BUILD_DUMP_AST "Build logera ast dumper" ON)

if(LOGERA_BUILD_DUMP_AST)
    add_subdirectory(dump_ast)
endif()
