#include "settings_dialog.h"
#include "ui_settings_dialog.h"

#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include <QLineEdit>
#include <QList>
#include <QSettings>

#include <memory>

#include "global_consts.h"
#include "main_window.h"
#include "little_helpers.h"

SettingsDialog::SettingsDialog(QWidget* parent)
    : QMainWindow(parent)
    , ui_(std::make_unique<Ui::SettingsDialog>())
    , parent_(dynamic_cast<MainWindow*>(parent))
{
    ui_->setupUi(this);

    connect(ui_->exit_button, &QPushButton::clicked, this, &SettingsDialog::onCloseClicked);
    connect(ui_->english_radio_button, &QRadioButton::toggled, this, &SettingsDialog::onEnglishSelected);
    connect(ui_->dark_radio_button, &QRadioButton::toggled, this, &SettingsDialog::onDarkSelected);
}

SettingsDialog::~SettingsDialog()
{
}

void SettingsDialog::onCloseClicked()
{
    close();
}

void SettingsDialog::onEnglishSelected(bool checked)
{
    auto const lang = checked ? lang_values::EN : lang_values::RU;
    parent_->switchLanguage(lang);
    parent_->getSettings()->setValue(conf_param_name::LANG, lang);
}

void SettingsDialog::onDarkSelected(bool checked)
{
    auto const theme = checked ? theme_values::DARK : theme_values::LIGHT;
    parent_->switchTheme(theme);
    parent_->getSettings()->setValue(conf_param_name::THEME, theme);
}

void SettingsDialog::updateTranslatable()
{
    ui_->exit_button->setText(tr_values::SETTINGS_EXIT_BUTTON());
    this->setWindowTitle(tr_values::SETTINGS_WINDOW_TITLE());

    ui_->hotkeys_group->setTitle(tr_values::SETTINGS_HOTKEYS_GROUP());
    ui_->lang_group->setTitle(tr_values::SETTINGS_LANG_GROUP());
    ui_->theme_group->setTitle(tr_values::SETTINGS_THEME_GROUP());
    ui_->light_radio_button->setText(tr_values::SETTINGS_LIGHT_RADIO());
    ui_->dark_radio_button->setText(tr_values::SETTINGS_DARK_RADIO());

    ui_->label_open->setText(tr_values::SETTINGS_OPEN_LABEL());
    ui_->label_save->setText(tr_values::SETTINGS_SAVE_LABEL());
    ui_->label_new->setText(tr_values::SETTINGS_NEW_LABEL());
    ui_->label_quit->setText(tr_values::SETTINGS_QUIT_LABEL());
}

void SettingsDialog::switchTheme(QString const& theme)
{
    auto lst = this->findChildren<QWidget*>();
    lst.push_back(this);

    processStyleInList(lst, theme);
}

bool SettingsDialog::getLanguage() const
{
    return ui_->english_radio_button->isChecked();
}

QString SettingsDialog::getTheme() const
{
    return ui_->dark_radio_button->isChecked()
            ? theme_values::DARK
            : theme_values::LIGHT;
}

void SettingsDialog::setLanguageRadioButton(QString const& language)
{
    ui_->english_radio_button->setChecked(language == lang_values::EN);
}

void SettingsDialog::setThemeRadioButton(const QString &theme)
{
    ui_->dark_radio_button->setChecked(theme == theme_values::DARK);
}

QLineEdit* SettingsDialog::getOpenEdit()
{
    return ui_->open_edit;
}

QLineEdit* SettingsDialog::getSaveEdit()
{
    return ui_->save_edit;
}

QLineEdit* SettingsDialog::getNewEdit()
{
    return ui_->new_edit;
}

QLineEdit* SettingsDialog::getQuitEdit()
{
    return ui_->quit_edit;
}
