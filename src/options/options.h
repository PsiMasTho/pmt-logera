#ifndef INCLUDED_OPTIONS_
#define INCLUDED_OPTIONS_

#include <string>
#include <vector>
#include <filesystem>
#include <regex>
#include <memory>
#include <iosfwd>

class Args;

enum FilterType
{
    INCLUSIVE,
    EXCLUSIVE,
    NONE
};

class Options
{
    static std::unique_ptr<Options>          s_instance;

    std::filesystem::path const              d_headerFile;
    std::filesystem::path const              d_outputFile;
    std::vector<std::filesystem::path> const d_logFiles;
    FilterType const                         d_filterType;
    std::regex const                         d_filterRegex;

public:
    static Options& initialize(Args const& args);
    static Options& instance();
    Options(Options const&) = delete;

        // accessors
    std::filesystem::path const& headerFile() const;
    std::filesystem::path const& outputFile() const;
    std::vector<std::filesystem::path> logFiles() const;
    FilterType filterType() const;
    std::regex const& filterRegex() const;

        // print
    void debugPrint(std::ostream& out) const;
        
private:
    Options(Args const& args);

        // converts a string to a given type e.g. string of space seperated
        // paths to a vector of filesystem::path
    template <typename Type>
    static Type cvtFunc(std::string const& valStr);

        // assigning the member variables from the strings in Args
    decltype(d_headerFile)  setHeaderFile(Args const& args);
    decltype(d_outputFile)  setOutputFile(Args const& args);
    decltype(d_logFiles)    setLogFiles(Args const& args);
    decltype(d_filterType)  setFilterType(Args const& args);
    decltype(d_filterRegex) setFilterRegex(Args const& args);
};

#endif