#include "html_parser.h"

#include <QDebug>
#include <QString>
#include <string>
#include <unordered_map>
#include <utility>
#include <regex>
#include <algorithm>
#include <cctype>

// Exercise 3.2
std::unordered_map<std::string, std::string> HtmlParser::Parse(QString const& text) const
{
    auto const str = text.toStdString();
    std::unordered_map<std::string, std::string> info;

    if (std::regex_search(str, block().second, block().first))
    {
        auto const blk = block().second[1].str();
        for (auto&& [key, value] : templates())
        {
            if (std::regex_search(blk, value.second, value.first))
            {
                auto const idx = value.second.size() - 1;
                auto s = value.second[idx].str();
                info[key] = rtrim(s);
            }
        }
    }
    return info;
}

HtmlParser::rgx_t& HtmlParser::block()
{
    static rgx_t r
    {
        std::regex(R"(class=\"weather ([\s\S]*?)class=\"horoscope)"),
        std::smatch()
    };
    return r;
}

HtmlParser::map_rgx_t& HtmlParser::templates()
{
    static map_rgx_t m
    {
        { "city", std::make_pair(std::regex(R"(data-testid=\"weather-city\">([\s\S]*?)<\/a>)"), std::smatch()) },
        { "temp", std::make_pair(std::regex(R"(data-testid=\"weather-temp\">([\s\S]*?)<\/span>)"), std::smatch()) },
        { "feel", std::make_pair(std::regex(R"(title=\"ощущается как (.*?)\")"), std::smatch()) },
        { "desc", std::make_pair(std::regex(R"(<div class=\"weather__description (.*?)\">([\s\S]*?)<\/div>)"), std::smatch()) },
        { "humi", std::make_pair(std::regex(R"(<div class=\"weather__humidity (.*?)\">Влажность ([\s\S]*?)<\/div>)"), std::smatch()) },
        { "usd", std::make_pair(std::regex(R"(<div class=\"rates ([\s\S]*?)USD([\s\S]*?)<div class=\"rate__currency (.*?)>([\s\S]*?)<\/div>)"), std::smatch()) },
        { "eur", std::make_pair(std::regex(R"(<div class=\"rates ([\s\S]*?)EUR([\s\S]*?)<div class=\"rate__currency (.*?)>([\s\S]*?)<\/div>)"), std::smatch()) },
        { "usd_change", std::make_pair(std::regex(R"(<div class=\"rates ([\s\S]*?)USD([\s\S]*?)<div class=\"rate__diff (.*?)>([\s\S]*?)<img class=\"rate__diff-icon)"), std::smatch()) },
        { "eur_change", std::make_pair(std::regex(R"(<div class=\"rates ([\s\S]*?)EUR([\s\S]*?)<div class=\"rate__diff (.*?)>([\s\S]*?)<img class=\"rate__diff-icon)"), std::smatch()) }
    };
    return m;
}

std::string& HtmlParser::rtrim(std::string& str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(),
                           [](auto c) { return !std::isspace(c); }).base(),
              str.end());
    return str;
}
