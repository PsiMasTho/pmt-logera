#ifndef INCLUDED_OPTIONS_
#define INCLUDED_OPTIONS_

#include <string>
#include <vector>
#include <filesystem>
#include <regex>
#include <memory>
#include <iosfwd>

class Args;

enum FilterType { INCLUSIVE, EXCLUSIVE, NONE };

class Options {
    bool                                     d_verbose;
    std::filesystem::path const              d_headerFile;
    std::filesystem::path const              d_outputFile;
    std::vector<std::filesystem::path> const d_logFiles;

public:
    explicit Options(Args const& args);

    bool good() const;

    // accessors
    decltype(d_verbose) const&    verbose() const;
    decltype(d_headerFile) const& headerFile() const;
    decltype(d_outputFile) const& outputFile() const;
    decltype(d_logFiles) const&   logFiles() const;

    // print
    void verbosePrint(std::ostream& out) const;

private:
    // converts a string to a given type e.g. string of space seperated
    // paths to a vector of filesystem::path
    template <typename Type> static Type cvtFunc(std::string const& valStr);

    // assigning the member variables from the strings in Args
    decltype(d_verbose)    setVerbose(Args const& args);
    decltype(d_headerFile) setHeaderFile(Args const& args);
    decltype(d_outputFile) setOutputFile(Args const& args);
    decltype(d_logFiles)   setLogFiles(Args const& args);
};

#endif