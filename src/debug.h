#ifndef INCLUDED_DEBUG_H
#define INCLUDED_DEBUG_H

#include <filesystem>

void scanHeader(std::filesystem::path const& pth);
void scanLog(std::filesystem::path const& pth);

#endif