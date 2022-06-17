#pragma once

#include <QString>

#include <utility>
#include <regex>
#include <string>
#include <vector>

class HtmlParser
{
public:
    HtmlParser() = delete;
    static std::vector<QString> Parse(QString const& text);

private:
    using rgxT = std::pair<std::regex, std::smatch>;

    static rgxT& templ();
    static std::string& rtrim(std::string& str);
};
