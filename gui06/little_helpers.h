#pragma once

#include <vector>

#include <QList>
#include <QWidget>
#include <QString>
#include <QStyle>

#include "global_consts.h"

inline void processStyleInList(QList<QWidget*>& lst, QString const& theme)
{
    for (auto&& child : lst)
    {
        child->setProperty(qss_property_name::THEME, theme);
        child->style()->unpolish(child);
        child->style()->polish(child);
    }
}

template<typename T>
inline bool checkIndex(int idx, std::vector<T> const& vec)
{
    return idx != -1 && static_cast<std::size_t>(idx) < vec.size();
}
