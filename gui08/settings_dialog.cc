#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include <QLineEdit>
#include <QList>

#include <memory>

#include "global_consts.h"
#include "little_helpers.h"
#include "main_window.h"
#include "key_event_filter.h"

SettingsDialog::SettingsDialog(QWidget* parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::SettingsDialog>())
{
    ui->setupUi(this);
    ui->open_edit->setAccessibleName(hotkeys_names::OPEN);
    ui->save_edit->setAccessibleName(hotkeys_names::SAVE);
    ui->new_edit->setAccessibleName(hotkeys_names::_NEW);
    ui->quit_edit->setAccessibleName(hotkeys_names::QUIT);

    connect(ui->exit_button, &QPushButton::clicked, this, &SettingsDialog::onCloseClicked);
    connect(ui->english_radio_button, &QRadioButton::toggled, this, [this](auto checked)
    {
        emit englishSelected(checked);
    });
    connect(ui->dark_radio_button, &QRadioButton::toggled, this, [this](auto checked)
    {
        emit darkSelected(checked);
    });
    auto const main = dynamic_cast<MainWindow*>(parent);
    connect(main, &MainWindow::translate, this, &SettingsDialog::onTranslate);
    connect(main, &MainWindow::changeTheme, this, &SettingsDialog::onChangeTheme);
    connect(main, &MainWindow::updateLanguageSelector, this,
            &SettingsDialog::onUpdateLanguageSelector);
    connect(main, &MainWindow::updateThemeSelector, this, &SettingsDialog::onUpdateThemeSelector);
    connect(main, &MainWindow::updateLineEditPlaceholderText, this,
            &SettingsDialog::onUpdateLineEditPlaceholderText);

    evFilter = new KeyEventFilter(this);
    ui->open_edit->installEventFilter(evFilter);
    ui->save_edit->installEventFilter(evFilter);
    ui->new_edit->installEventFilter(evFilter);
    ui->quit_edit->installEventFilter(evFilter);
    connect(evFilter, &KeyEventFilter::changeHotKey, this, [this](auto const& name, auto const& seq)
    {
        emit changeHotKey(name, seq);
    });
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::onCloseClicked()
{
    close();
}

void SettingsDialog::onTranslate()
{
    ui->exit_button->setText(tr_values::SETTINGS_EXIT_BUTTON());
    this->setWindowTitle(tr_values::SETTINGS_WINDOW_TITLE());

    ui->hotkeys_group->setTitle(tr_values::SETTINGS_HOTKEYS_GROUP());
    ui->lang_group->setTitle(tr_values::SETTINGS_LANG_GROUP());
    ui->theme_group->setTitle(tr_values::SETTINGS_THEME_GROUP());
    ui->light_radio_button->setText(tr_values::SETTINGS_LIGHT_RADIO());
    ui->dark_radio_button->setText(tr_values::SETTINGS_DARK_RADIO());

    ui->label_open->setText(tr_values::SETTINGS_OPEN_LABEL());
    ui->label_save->setText(tr_values::SETTINGS_SAVE_LABEL());
    ui->label_new->setText(tr_values::SETTINGS_NEW_LABEL());
    ui->label_quit->setText(tr_values::SETTINGS_QUIT_LABEL());
}

void SettingsDialog::onChangeTheme(QString const& theme)
{
    auto lst = this->findChildren<QWidget*>();
    lst.push_back(this);

    processStyleInList(lst, theme);
}

void SettingsDialog::onUpdateLanguageSelector(QString const& language)
{
    ui->english_radio_button->setChecked(language == lang_values::EN);
}

void SettingsDialog::onUpdateThemeSelector(QString const& theme)
{
    ui->dark_radio_button->setChecked(theme == theme_values::DARK);
}

void SettingsDialog::onUpdateLineEditPlaceholderText(QString const& name, QString const& text)
{
    auto const edits = this->findChildren<QLineEdit*>();
    for (auto edit : edits)
    {
        if (edit->accessibleName() == name)
        {
            edit->setPlaceholderText(text);
        }
    }
}
