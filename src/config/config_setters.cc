#include "config.ih"

decltype(Config::d_verbose) Config::setVerbose(Args const& args)
{
    return args.option('v').first;
}

decltype(Config::d_headerFile) Config::setHeaderFile(Args const& args)
{
    auto const [specified_d, str_d] = args.option('d');
    auto const [specified_m, str_m] = args.option('m');

    if(specified_m && specified_d)
        throw invalid_argument("Specify either -m or -d");

    if(!specified_d && !specified_m)
        throw invalid_argument("Specify either -m or -d");

    filesystem::path ret;

    if(specified_m)
    {
        auto pathVec = cvtFunc<vector<filesystem::path>>(str_m);
        auto itr = find_if(
            begin(pathVec), end(pathVec), [](auto pth) { return pth.extension() == ".lh"; });

        if(itr != end(pathVec))
            ret = *itr;
        else
            throw invalid_argument("No header path provided");
    }
    else if(specified_d)
    {
        filesystem::directory_iterator itrBeg(cvtFunc<filesystem::path>(str_d));

        auto const itr = find_if(itrBeg, filesystem::directory_iterator{}, [](auto entry) {
            return entry.path().extension() == ".lh";
        });

        if(itr != filesystem::directory_iterator{})
            ret = *itr;
        else
            throw invalid_argument("No header path provided");
    }

    return ret;
}

decltype(Config::d_outputFile) Config::setOutputFile(Args const& args)
{
    auto const [specified_o, str_o] = args.option('o');

    if(!specified_o)
        return filesystem::path{"out.csv"s};
    else
        return cvtFunc<filesystem::path>(str_o);
}

decltype(Config::d_logFiles) Config::setLogFiles(Args const& args)
{
    auto const [specified_d, str_d] = args.option('d');
    auto const [specified_m, str_m] = args.option('m');

    if(specified_m && specified_d)
        throw invalid_argument("Specify either -m or -d");

    if(!specified_m && !specified_d)
        throw invalid_argument("Specify either -m or -d");

    vector<filesystem::path> ret;

    if(specified_m)
    {
        ret = cvtFunc<vector<filesystem::path>>(str_m);
        ret.erase(
            remove_if(begin(ret), end(ret), [](auto pth) { return pth.extension() != ".txt"; }),
            end(ret));
    }
    else if(specified_d)
    {
        filesystem::directory_iterator const itrBeg(cvtFunc<filesystem::path>(str_d));

        for(auto itr = itrBeg; itr != filesystem::directory_iterator{}; ++itr)
            if(itr->path().extension() == ".txt")
                ret.push_back(itr->path());
    }

    if(ret.empty())
        throw invalid_argument("No log files provided");

    return ret;
}