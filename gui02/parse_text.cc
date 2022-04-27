#include "parse_text.h"

#include <QDebug>
#include <string>
#include <vector>
#include <utility>
#include <regex>


// Exercise 1
std::vector<std::string> ParseText::ToReplace(QString const& text) const
{
    auto const str = text.toStdString();
    std::vector<std::string> specs;
    for (auto&& [key, value] : spec_chars())
    {
        if (std::regex_search(str, value.second, value.first))
        {
            specs.emplace_back(key);
        }
    }
    return specs;
}


void ParseText::Parse(QString& text, std::vector<std::string> const& specs)
{
    auto str = text.toStdString();
    for (auto const& c : specs)
    {
        str = std::regex_replace(str, spec_chars()[c].first, c);
    }
    text = QString::fromStdString(str);
}


ParseText::map_rgx_t& ParseText::spec_chars()
{
    static map_rgx_t specs {
        { "₽", std::make_pair(std::regex("руб\\.|\\#\\@RUB\\@"), std::smatch()) },
        { "€", std::make_pair(std::regex("евро|\\#\\@EUR\\@"), std::smatch()) },
        { "©", std::make_pair(std::regex("\\#\\@COP\\@"), std::smatch()) },
        { "‰", std::make_pair(std::regex("\\#\\@PPM\\@"), std::smatch()) },
        { "®", std::make_pair(std::regex("\\#\\@REG\\@"), std::smatch()) }
    };
    return specs;
}
