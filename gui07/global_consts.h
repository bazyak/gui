#pragma once

#include <QObject>
#include <QString>

namespace glob_values
{
QString inline const CONF_FILE_NAME { "settings.conf" };
QString inline const TR_FILE_PREFIX { ":/gui_" };
QString inline const HELP_FILE_PREFIX { ":/help_" };
QString inline const HELP_FILE_SUFFIX { ".txt" };
}

namespace hotkeys_values
{
QString inline const OPEN { "Ctrl+O" };
QString inline const SAVE { "Ctrl+S" };
QString inline const _NEW { "Ctrl+N" };
QString inline const QUIT { "Ctrl+Q" };
}

namespace tr_values
{
auto inline WINDOW_TITLE() { return QObject::tr("Урок 07"); }
auto inline NEW_BTN() { return QObject::tr("Новый"); }
auto inline OPEN_BTN() { return QObject::tr("Открыть"); }
auto inline OPEN_READ_BTN() { return QObject::tr("Открыть на чтение"); }
auto inline SAVE_BTN() { return QObject::tr("Сохранить"); }
auto inline PRINT_BTN() { return QObject::tr("Печать"); }
auto inline SETTINGS_BTN() { return QObject::tr("Настройки"); }
auto inline HELP_BTN() { return QObject::tr("Справка"); }
auto inline LEFT_BTN() { return QObject::tr("Л"); }
auto inline CENTER_BTN() { return QObject::tr("Ц"); }
auto inline RIGHT_BTN() { return QObject::tr("П"); }
auto inline FONT_BTN() { return QObject::tr("Шрифт"); }
auto inline COPY_FMT_BTN() { return QObject::tr("Скопировать формат"); }
auto inline APPLY_FMT_BTN() { return QObject::tr("Применить формат"); }

auto inline FILE_MENU() { return QObject::tr("Файл"); }
auto inline EDIT_MENU() { return QObject::tr("Правка"); }
auto inline COPY_ACTION() { return QObject::tr("Копировать"); }
auto inline CUT_ACTION() { return QObject::tr("Вырезать"); }
auto inline PASTE_ACTION() { return QObject::tr("Вставить"); }

auto inline SETTINGS_EXIT_BUTTON() { return QObject::tr("Закрыть"); }
auto inline SETTINGS_WINDOW_TITLE() { return QObject::tr("Настройки"); }
auto inline SETTINGS_HOTKEYS_GROUP() { return QObject::tr("Горячие клавиши"); }
auto inline SETTINGS_LANG_GROUP() { return QObject::tr("Язык"); }
auto inline SETTINGS_THEME_GROUP() { return QObject::tr("Оформление"); }
auto inline SETTINGS_LIGHT_RADIO() { return QObject::tr("Светлое"); }
auto inline SETTINGS_DARK_RADIO() { return QObject::tr("Тёмное"); }
auto inline SETTINGS_OPEN_LABEL() { return QObject::tr("Открыть"); }
auto inline SETTINGS_SAVE_LABEL() { return QObject::tr("Сохранить"); }
auto inline SETTINGS_NEW_LABEL() { return QObject::tr("Новый"); }
auto inline SETTINGS_QUIT_LABEL() { return QObject::tr("Выход"); }

auto inline SAVE_DLG_TITLE() { return QObject::tr("Выберите файл для сохранения"); }
auto inline SAVE_DLG_FILE_TYPES() { return QObject::tr("Html файл (*.html);;Любой файл (*.*)"); }
auto inline SAVE_DLG_ERR_TITLE() { return QObject::tr("Ошибка"); }
auto inline SAVE_DLG_ERR_TEXT() { return QObject::tr("Этот файл открыт только для чтения"); }

auto inline OPEN_DLG_TITLE() { return QObject::tr("Выберите файл для открытия"); }
auto inline OPEN_DLG_FILE_TYPES() { return QObject::tr("Html файлы (*.html);;Все файлы (*.*)"); }

auto inline TAB_DEF_TITLE() { return QObject::tr("без имени"); }
auto inline TAB_READ_ONLY_SUFFIX() { return QObject::tr("[Только для чтения]"); }

auto inline HELP_DLG_TITLE() { return QObject::tr("Помощь"); }

auto inline PRINT_DLG_TITLE() { return QObject::tr("Печать"); }

auto inline CLOSE_TAB_TITLE() { return QObject::tr("Сохранить?"); }
auto inline CLOSE_TAB_TEXT() { return QObject::tr("Сохранить файл перед закрытием?"); }

auto inline FINDER_MENU() { return QObject::tr("Задание 5.3"); }
auto inline FINDER_ACTION() { return QObject::tr("Просмотрщик"); }
auto inline FINDER_CLOSE_BTN() { return QObject::tr("Закрыть"); }
}

namespace conf_param_name
{
QString inline const DIR { "dir" };
QString inline const LANG { "lang" };
char constexpr THEME[] { "theme" };
}

namespace lang_values
{
QString inline const RU { "ru" };
QString inline const EN { "en" };
}

namespace theme_values
{
QString inline const LIGHT { "light" };
QString inline const DARK { "dark" };
}

namespace qss_styles
{
QString inline const MAIN
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
    QTextEdit { \
        background-color: #ffffff; \
    } \
    QTextEdit[theme=light] { \
        background-color: #ffffff; \
    } \
    QTextEdit[theme=dark] { \
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
