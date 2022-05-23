#pragma once

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
