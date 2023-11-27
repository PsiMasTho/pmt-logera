#pragma once

#include "archive_data.h"

#include <string>
#include <vector>
#include <regex>

class regex_matcher
{
    std::vector<std::regex> m_regexes;

public:
    void add_regex(std::string const& expr);
    auto operator()(std::string const& str) const -> bool;
};

auto construct_regex_matchers(header_data const& hd) -> std::vector<regex_matcher>;