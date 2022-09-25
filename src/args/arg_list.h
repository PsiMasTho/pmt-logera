#ifndef  CLI_ARG_OPT
    #define CLI_ARG_OPT(letter, name, Type, defaultVal)
#endif

    // single quotes around character X
#define SQ(X) ((#X)[0])

    // note: the defaultVal is an std::optional
CLI_ARG_OPT(SQ(m), inputFiles,      std::vector<std::filesystem::path>, std::nullopt)
CLI_ARG_OPT(SQ(d), inputFilesDir,   std::filesystem::path             , std::nullopt)
CLI_ARG_OPT(SQ(o), outputFile,      std::filesystem::path             , std::nullopt)

CLI_ARG_OPT(SQ(I), inclusiveFilter, InclusiveRegexFilter              , std::nullopt)
CLI_ARG_OPT(SQ(E), exclusiveFilter, ExclusiveRegexFilter              , std::nullopt)
#undef CLI_ARG_OPT
#undef SQ