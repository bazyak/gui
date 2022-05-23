#pragma once

#include <QString>

namespace qss_property_name
{
char constexpr THEME[] { "theme" };
}

namespace theme_values
{
QString const LIGHT { "light" };
QString const DARK { "dark" };
}

namespace qss_styles
{
static inline QString const MAIN
{
    "QLabel { \
        color: black; \
    } \
    QLabel[theme=light] { \
        color: black; \
    } \
    QLabel[theme=dark] { \
        color: white; \
    } \
    QMenu::item:selected { \
        background: blue; \
    } \
    QMenu::item:selected[theme=light] { \
        background: blue; \
    } \
    QMenu[theme=dark] { \
        color: white; \
    } \
    QMenu::item:selected[theme=dark] { \
        background: #ececec; \
        color: black; \
    } \
    QDialog { \
        background-color: #ececec; \
    } \
    QDialog[theme=light] { \
        background-color: #ececec; \
    } \
    QDialog[theme=dark] { \
        background-color: #666666; \
    } \
    QWidget { \
        background-color: #ececec; \
    } \
    QWidget[theme=light] { \
        background-color: #ececec; \
    } \
    QWidget[theme=dark] { \
        background-color: #666666; \
    } \
    QTreeView { \
        background-color: #ffffff; \
    } \
    QTreeView[theme=light] { \
        background-color: #ffffff; \
    } \
    QTreeView[theme=dark] { \
        background-color: darkgrey; \
        color: white; \
    } \
    QPlainTextEdit { \
        background-color: #ffffff; \
    } \
    QPlainTextEdit[theme=light] { \
        background-color: #ffffff; \
    } \
    QPlainTextEdit[theme=dark] { \
        background-color: darkgrey; \
        color: white; \
    } \
    QPushButton { \
        background-color: #ffffff; \
        border-radius: 6px; \
        padding: 2px; \
        border: 1px solid #cccccc; \
        font-size: 13px; \
    } \
    QPushButton[theme=light] { \
        background-color: #ffffff; \
        border-radius: 6px; \
        padding: 2px; \
        border: 1px solid #cccccc; \
        font-size: 13px; \
    } \
    QPushButton[theme=dark] { \
        background-color: darkgrey; \
        border-radius: 6px; \
        padding: 2px; \
        border: 1px solid #333333; \
        font-size: 13px; \
        color: white; \
    } \
    QGroupBox { \
        background-color: #ececec; \
        font-size: 11px; \
    } \
    QGroupBox * { \
        background-color: #e5e5e5; \
    } \
    QGroupBox[theme=light] { \
        background-color: #ececec; \
        font-size: 11px; \
    } \
    QGroupBox[theme=light] * { \
        background-color: #e5e5e5; \
    } \
    QGroupBox[theme=dark] { \
        background-color: #666666; \
        color: white; \
        font-size: 11px; \
    } \
    QGroupBox[theme=dark] * { \
        background-color: #636363; \
        color: white; \
    } \
    QLineEdit { \
        background-color: #ffffff; \
    } \
    QLineEdit[theme=light] { \
        background-color: #ffffff; \
    } \
    QLineEdit[theme=dark] { \
        background-color: darkgrey; \
        border: 1px solid #333333; \
    }"
};
}
