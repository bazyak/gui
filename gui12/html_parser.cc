#include "html_parser.h"

#include <QDebug>
#include <QString>

#include <string>
#include <utility>
#include <regex>
#include <algorithm>
#include <cctype>
#include <vector>

std::vector<QString> HtmlParser::Parse(QString const& text)
{
    auto toParse = text.toStdString();
    std::vector<QString> items { };

    auto i = 0;
    while (regex_search(toParse, templ().second, templ().first) && i++ < 3)
    {
        auto item = std::string(templ().second[1].str());
        items.push_back(QString::fromStdString(rtrim(item)));
        toParse = templ().second.suffix();
    }

    return items;
}

HtmlParser::rgxT& HtmlParser::templ()
{
    static rgxT rgx
    {
        std::regex(R"(<img class=\"serp-item__thumb justifier__thumb\" src=\"([\s\S]*?)\")"),
        std::smatch()
    };
    return rgx;
}

std::string& HtmlParser::rtrim(std::string& str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(),
                           [](auto c) { return !std::isspace(c); }).base(),
              str.end());
    return str;
}
