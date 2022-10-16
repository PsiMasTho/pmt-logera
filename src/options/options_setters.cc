#include "options.ih"

using namespace std;

decltype(Options::d_verbose) Options::setVerbose(Args const& args)
{
    bool specified_v = false;

    args.option(&specified_v, 'v');

    if (specified_v)
        return true;
    else
        return false;
}

decltype(Options::d_headerFile) Options::setHeaderFile(Args const& args)
{
    bool specified_d = false;
    bool specified_m = false;
    
    string const& str_d = args.option(&specified_d, 'd');
    string const& str_m = args.option(&specified_m, 'm');

    if (specified_m && specified_d)
        throw invalid_argument("Specify either -m or -d");

    if (!specified_d && !specified_m)
        throw invalid_argument("Specify either -m or -d");
    
    filesystem::path ret;

    if (specified_m)
    {
        auto pathVec = cvtFunc<vector<filesystem::path>>(str_m);
        auto itr = find_if(begin(pathVec), end(pathVec), [](auto pth){return pth.extension() == ".lh";});
        
        if (itr != end(pathVec))
            ret = *itr;
        else
            throw invalid_argument("No header path provided");
    }
    else if (specified_d)
    {
        filesystem::directory_iterator itrBeg(cvtFunc<filesystem::path>(str_d));

        auto itr = find_if(itrBeg, filesystem::directory_iterator{}, [](auto entry){return entry.path().extension() == ".lh";});
        
        if (itr != filesystem::directory_iterator{})
            ret = *itr;
        else
            throw invalid_argument("No header path provided");
    }

    return ret;
}

decltype(Options::d_outputFile) Options::setOutputFile(Args const& args)
{
    filesystem::path ret;

    bool specified_o = false;
    string const& str_o = args.option(&specified_o, 'o');

    if (!specified_o)
        ret = filesystem::path{"out.csv"s};
    else
        ret = cvtFunc<filesystem::path>(str_o);

    return ret;
}

decltype(Options::d_logFiles) Options::setLogFiles(Args const& args)
{
    bool specified_d = false;
    bool specified_m = false;
    
    string const& str_d = args.option(&specified_d, 'd');
    string const& str_m = args.option(&specified_m, 'm');

    if (specified_m && specified_d)
        throw invalid_argument("Specify either -m or -d");

    if (!specified_m && !specified_d)
        throw invalid_argument("Specify either -m or -d");
    
    vector<filesystem::path> ret;

    if (specified_m)
    {
        ret = cvtFunc<vector<filesystem::path>>(str_m);
        ret.erase(remove_if(begin(ret), end(ret), [](auto pth){return pth.extension() != ".txt";}));
    }
    else if (specified_d)
    {
        filesystem::directory_iterator itrBeg(cvtFunc<filesystem::path>(str_d));

        for (auto itr = itrBeg; itr != filesystem::directory_iterator{}; ++itr)
            if (itr->path().extension() == ".txt")
                ret.push_back(itr->path());
    }

    if (ret.empty())
        throw invalid_argument("No log files provided");

    return ret;
}

