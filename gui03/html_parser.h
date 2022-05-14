#pragma once

#include <QString>
#include <utility>
#include <unordered_map>
#include <regex>
#include <string>

// Exercise 3.2
class HtmlParser
{
public:
    HtmlParser() = default;

    std::unordered_map<std::string, std::string> Parse(QString const& text) const;

private:
    using rgx_t = std::pair<std::regex, std::smatch>;
    using map_rgx_t = std::unordered_map<std::string, rgx_t>;

    static rgx_t& block();
    static map_rgx_t& templates();

    static std::string& rtrim(std::string& str);
};
