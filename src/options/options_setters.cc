#include "options.ih"

using namespace std;

decltype(Options::d_headerFile)  Options::setHeaderFile(Args const& args)
{
    if (args.option('m') && args.option('d'))
        throw "Specify either -m or -d";

    if (!args.option('m') && !args.option('d'))
        throw "Specify either -m or -d";
    
    filesystem::path ret;

    if (args.option('m'))
    {
        auto pathVec = cvtFunc<vector<filesystem::path>>(*args.option('m'));
        auto itr = find_if(begin(pathVec), end(pathVec), [](auto pth){return pth.extension() == ".lh";});
        
        if (itr != end(pathVec))
            ret = *itr;
        else
            throw "No header path provided";
    }
    else if (args.option('d'))
    {
        filesystem::directory_iterator itrBeg(cvtFunc<filesystem::path>(*args.option('d')));

        auto itr = find_if(itrBeg, filesystem::directory_iterator{}, [](auto entry){return entry.path().extension() == ".lh";});
        
        if (itr != filesystem::directory_iterator{})
            ret = *itr;
        else
            throw "No header path provided";
    }

    return ret;
}

decltype(Options::d_outputFile)  Options::setOutputFile(Args const& args)
{
    filesystem::path ret;
    if (!args.option('o'))
        ret = filesystem::path{"out.csv"s};
    else
        ret = cvtFunc<filesystem::path>(*args.option('o'));

    return ret;
}

decltype(Options::d_logFiles)    Options::setLogFiles(Args const& args)
{
    if (args.option('m') && args.option('d'))
        throw "Specify either -m or -d";

    if (!args.option('m') && !args.option('d'))
        throw "Specify either -m or -d";
    
    vector<filesystem::path> ret;

    if (args.option('m'))
    {
        ret = cvtFunc<vector<filesystem::path>>(*args.option('m'));
        ret.erase(remove_if(begin(ret), end(ret), [](auto pth){return pth.extension() != ".txt";}));
    }
    else if (args.option('d'))
    {
        filesystem::directory_iterator itrBeg(cvtFunc<filesystem::path>(*args.option('d')));

        for (auto itr = itrBeg; itr != filesystem::directory_iterator{}; ++itr)
            if (itr->path().extension() == ".txt")
                ret.push_back(itr->path());
    }

    if (ret.empty())
        throw "No log files provided";

    return ret;
}

decltype(Options::d_filterType)  Options::setFilterType(Args const& args)
{
    if (args.option('E') && args.option('I'))
        throw "Specify one or none of: -E <expr> or -I <expr>";

    if (args.option('E') || args.option('I'))
        return args.option('E') ? FilterType::EXCLUSIVE : FilterType::INCLUSIVE;

    return FilterType::NONE;
}

decltype(Options::d_filterRegex) Options::setFilterRegex(Args const& args)
{
    if (args.option('E') && args.option('I'))
        throw "Specify one or none of: -E <expr> or -I <expr>";

    regex ret;

    if (args.option('E'))
        ret = cvtFunc<regex>(*args.option('E'));
    else if (args.option('I'))
        ret = cvtFunc<regex>(*args.option('I'));

    return ret;
}

