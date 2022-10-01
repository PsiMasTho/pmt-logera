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

decltype(Options::d_logFiles)    Options::setLogFiles(Args const& args)
{
	return {};
}

decltype(Options::d_filterType)  Options::setFilterType(Args const& args)
{
	return {};
}

decltype(Options::d_filterRegex) Options::setFilterRegex(Args const& args)
{
	return {};
}

