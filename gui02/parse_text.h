#pragma once

#include <QString>
#include <utility>
#include <unordered_map>
#include <regex>
#include <string>
#include <vector>


// Exercise 1
class ParseText
{
public:
    ParseText() = default;

    std::vector<std::string> toReplace(QString const& text) const;
    void parse(QString& text, std::vector<std::string> const& specs);

private:
    using map_rgx_t = std::unordered_map<std::string, std::pair<std::regex, std::smatch>>;

    static map_rgx_t& specChars();
};
