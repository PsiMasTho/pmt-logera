#ifndef INCLUDED_CONFIG_CONFIG_H
#define INCLUDED_CONFIG_CONFIG_H

#include <filesystem>
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

class Args;

class Config
{
    bool d_verbose;
    std::filesystem::path d_headerFile;
    std::unique_ptr<std::ofstream> d_outputStream;
    std::string d_outputName;
    std::vector<std::filesystem::path> d_logFiles;

public:
    explicit Config(Args const& args);

    // accessors
    bool verbose() const;
    std::filesystem::path const& headerFile() const;
    std::ostream& outputStream();
    std::vector<std::filesystem::path> const& logFiles() const;

    // print
    void verbosePrint(std::ostream& out) const;

private:
    // converts a string to a given type e.g. string of space seperated
    // paths to a vector of filesystem::path
    template <typename Type>
    static Type cvtFunc(std::string const& valStr);

    // assigning the member variables from the strings in Args
    bool setVerbose(Args const& args);
    bool setUseStdout(Args const& args);
    std::filesystem::path setHeaderFile(Args const& args);
    std::unique_ptr<std::ofstream> setOutputStream(Args const& args);
    std::string setOutputName(Args const& args);
    std::vector<std::filesystem::path> setLogFiles(Args const& args);
};

#endif