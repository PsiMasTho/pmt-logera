#include "options.ih"

std::unique_ptr<Options> Options::s_instance{nullptr};

Options& Options::initialize(Args const& args)
{
    s_instance.reset(new Options(args));
    return instance();
}

Options& Options::instance()
{
    if (s_instance)
        return *s_instance;
    else
        throw "Options not initialized before calling instance()";
}
 
Options::Options(Args const& args)
:
    d_headerFile{setHeaderFile(args)},
    d_outputFile{setOutputFile(args)},
    d_logFiles{setLogFiles(args)},
    d_filterType{setFilterType(args)},
    d_filterRegex{setFilterRegex(args)}
{}

