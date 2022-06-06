#include "main_window.h"
#include "ui_main_window.h"

#include <QApplication>
#include <QMainWindow>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include <QResource>
#include <QTranslator>
#include <QRadioButton>
#include <QSettings>
#include <QShortcut>
#include <QKeySequence>
#include <QStyle>
#include <QAction>
#include <QPrinter>
#include <QPrintDialog>
#include <QTabWidget>
#include <QFont>
#include <QFontDialog>
#include <QLineEdit>
#include <QDate>
#include <QTime>

#include <memory>

#include "global_consts.h"
#include "little_helpers.h"
#include "custom_text_edit.h"
#include "settings_dialog.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
    , settings(new QSettings(glob_values::CONF_FILE_NAME, QSettings::IniFormat, this))
    , settingsDialog(new SettingsDialog(this))
{
    ui->setupUi(this);
    Q_INIT_RESOURCE(res);
    setFixedSize(width(), height());

    initTabs();
    connectButtons();
    initShortcuts();
    prepareSettingsDialog();
}

MainWindow::~MainWindow()
{
}

void MainWindow::onOpenClicked()
{
    onNewClicked();
    auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
    if (!tab) return;

    if (checkIndex(curTabIdx, docs))
    {
        docs[curTabIdx].isReadOnly = false;
    }
    if (!loadFile(tab))
    {
        closeTab(curTabIdx, true);
    }
    updateBasedOnReadOnlyState();
}

void MainWindow::onOpenReadClicked()
{
    onNewClicked();
    auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
    if (!tab) return;

    if (checkIndex(curTabIdx, docs))
    {
        docs[curTabIdx].isReadOnly = true;
    }
    if (!loadFile(tab))
    {
        closeTab(curTabIdx, true);
    }
    updateBasedOnReadOnlyState();
}

void MainWindow::onSaveClicked()
{
    auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
    if (!tab) return;

    saveFile(tab, checkIndex(curTabIdx, docs) && docs[curTabIdx].filePath.isEmpty());
}

void MainWindow::onSaveAsClicked()
{
    auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
    if (!tab) return;

    saveFile(tab);
}

void MainWindow::onNewClicked()
{
    auto const tab = new CustomTextEdit(tabs);
    auto const idx = tabs->addTab(tab, tr_values::TAB_DEF_TITLE());
    docs.push_back({ });
    tabs->setCurrentIndex(idx);

    QList<QWidget*> lst { tab };

    processStyleInList(lst, theme);
}

void MainWindow::onHelpClicked()
{
    QFile file(glob_values::HELP_FILE_PREFIX + lang + glob_values::HELP_FILE_SUFFIX);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        auto const text = stream.readAll();
        QMessageBox::information(nullptr, tr_values::HELP_DLG_TITLE(), text);
    }
}

void MainWindow::onQuitClicked()
{
    qApp->exit();
}

void MainWindow::onPrintClicked()
{
    auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
    if (!tab) return;

    QPrinter printer;
    QPrintDialog dlg(&printer, this);
    dlg.setWindowTitle(tr_values::PRINT_DLG_TITLE());

    if (dlg.exec() != QDialog::Accepted) return;

    tab->print(&printer);
}

void MainWindow::onSettingsClicked()
{
    settingsDialog->show();
}

void MainWindow::tabSelected(int index)
{
    if (index != curTabIdx)
    {
        disconnect(ui->copy_action, &QAction::triggered, 0, 0);
        disconnect(ui->cut_action, &QAction::triggered, 0, 0);
        disconnect(ui->paste_action, &QAction::triggered, 0, 0);

        if (index != -1)
        {
            auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
            connect(ui->copy_action, &QAction::triggered, tab, &CustomTextEdit::copyText);
            connect(ui->cut_action, &QAction::triggered, tab, &CustomTextEdit::cutText);
            connect(ui->paste_action, &QAction::triggered, tab, &CustomTextEdit::pasteText);
            connect(tab, &CustomTextEdit::textChanged, this, &MainWindow::onTextChanged);
        }
        curTabIdx = index;
    }
}

void MainWindow::onLeftClicked()
{
    auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
    if (!tab) return;

    tab->setAlignment(Qt::AlignLeft);
}

void MainWindow::onCenterClicked()
{
    auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
    if (!tab) return;

    tab->setAlignment(Qt::AlignCenter);
}

void MainWindow::onRightClicked()
{
    auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
    if (!tab) return;

    tab->setAlignment(Qt::AlignRight);
}

void MainWindow::onFontClicked()
{
    auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
    if (!tab) return;

    QFont font = tab->textCursor().charFormat().font();
    bool flag { false };
    font = QFontDialog::getFont(&flag, font, nullptr, { }, QFontDialog::DontUseNativeDialog);

    if (flag)
    {
        if (!tab->textCursor().hasSelection())
        {
            auto fmt = tab->currentCharFormat();
            fmt.setFont(font);
            tab->setCurrentCharFormat(fmt);
        }
        auto fmt = tab->textCursor().charFormat();
        fmt.setFont(font);
        tab->textCursor().setCharFormat(fmt);
    }
}

void MainWindow::onCopyFormatClicked()
{
    auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
    if (!tab) return;

    if (tab->textCursor().hasSelection())
    {
        fmt.fmt = tab->textCursor().charFormat();
        fmt.align = tab->alignment();
    }
}

void MainWindow::onApplyFormatClicked()
{
    auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
    if (!tab) return;

    if (!tab->textCursor().hasSelection())
    {
        tab->setCurrentCharFormat(fmt.fmt);
    }
    else
    {
        tab->textCursor().setCharFormat(fmt.fmt);
    }
    tab->setAlignment(fmt.align);
}

void MainWindow::onDateClicked()
{
    auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
    if (!tab) return;

    auto const date { QDate::currentDate() };
    tab->textCursor().insertText(date.toString(" dd MMMM yyyy "));
}

void MainWindow::onTimeClicked()
{
    auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
    if (!tab) return;

    auto const time { QTime::currentTime() };
    tab->textCursor().insertText(time.toString(" hh:mm:ss "));
}

void MainWindow::onTextChanged()
{
    if (checkIndex(curTabIdx, docs) && docs[curTabIdx].isSaved)
    {
        auto const title = tabs->tabText(curTabIdx);
        tabs->setTabText(curTabIdx, "*" + title);
        docs[curTabIdx].isSaved = false;
    }
}

void MainWindow::onEnglishSelected(bool checked)
{
    lang = checked ? lang_values::EN : lang_values::RU;
    switchLanguage(lang);
    settings->setValue(conf_param_name::LANG, lang);
}

void MainWindow::onDarkSelected(bool checked)
{
    theme = checked ? theme_values::DARK : theme_values::LIGHT;
    switchTheme(theme);
    settings->setValue(conf_param_name::THEME, theme);
}

void MainWindow::onChangeHotKey(QString const& objName, QKeySequence const& keySeq)
{
    hotKeys[objName]->setKey(keySeq);
    settings->setValue(objName, keySeq.toString());
}

void MainWindow::switchLanguage(QString const& lang)
{
    if (translator.load(glob_values::TR_FILE_PREFIX + lang))
    {
        qApp->installTranslator(&translator);
    }
    this->setWindowTitle(tr_values::WINDOW_TITLE());

    ui->new_button->setText(tr_values::NEW_BTN());
    ui->open_button->setText(tr_values::OPEN_BTN());
    ui->open_read_button->setText(tr_values::OPEN_READ_BTN());
    ui->save_button->setText(tr_values::SAVE_BTN());
    ui->save_as_button->setText(tr_values::SAVE_AS_BTN());
    ui->print_button->setText(tr_values::PRINT_BTN());
    ui->settings_button->setText(tr_values::SETTINGS_BTN());
    ui->help_button->setText(tr_values::HELP_BTN());

    ui->file_menu->setTitle(tr_values::FILE_MENU());
    ui->edit_menu->setTitle(tr_values::EDIT_MENU());
    ui->open_action->setText(tr_values::OPEN_BTN());
    ui->open_rd_action->setText(tr_values::OPEN_READ_BTN());
    ui->save_action->setText(tr_values::SAVE_BTN());
    ui->print_action->setText(tr_values::PRINT_BTN());
    ui->help_action->setText(tr_values::HELP_BTN());
    ui->copy_action->setText(tr_values::COPY_ACTION());
    ui->cut_action->setText(tr_values::CUT_ACTION());
    ui->paste_action->setText(tr_values::PASTE_ACTION());

    ui->left_button->setText(tr_values::LEFT_BTN());
    ui->center_button->setText(tr_values::CENTER_BTN());
    ui->right_button->setText(tr_values::RIGHT_BTN());
    ui->font_button->setText(tr_values::FONT_BTN());
    ui->copy_format_button->setText(tr_values::COPY_FMT_BTN());
    ui->apply_format_button->setText(tr_values::APPLY_FMT_BTN());
    ui->date_button->setText(tr_values::DATE_BTN());
    ui->time_button->setText(tr_values::TIME_BTN());

    updateBasedOnReadOnlyState(false);
    emit translate();
}

void MainWindow::switchTheme(QString const& theme)
{
    auto lst = ui->centralwidget->findChildren<QWidget*>();
    lst.push_back(ui->centralwidget);

    processStyleInList(lst, theme);

    emit changeTheme(theme);
}

bool MainWindow::loadFile(CustomTextEdit* tab)
{
    if (!tab) return false;

    auto const fPath = QFileDialog::getOpenFileName(this,
                                                    tr_values::OPEN_DLG_TITLE(),
                                                    dir,
                                                    tr_values::OPEN_DLG_FILE_TYPES());
    if (fPath.length() > 0)
    {
        QFile file(fPath);
        if (file.open(QFile::ReadOnly | QFile::ExistingOnly))
        {
            QTextStream stream(&file);
            tab->setHtml(stream.readAll());

            dir = QFileInfo(fPath).absolutePath();
            settings->setValue(conf_param_name::DIR, dir);

            if (checkIndex(curTabIdx, docs))
            {
                docs[curTabIdx].filePath = fPath;
                docs[curTabIdx].isSaved = true;
            }
            return true;
        }
    }
    return false;
}

void MainWindow::saveFile(CustomTextEdit* tab, bool saveAs)
{
    if (!tab) return;

    if (!checkIndex(curTabIdx, docs)) return;

    QString fPath { docs[curTabIdx].filePath };
    if (saveAs)
    {
        fPath = QFileDialog::getSaveFileName(this, tr_values::SAVE_DLG_TITLE(),
                                             dir, tr_values::SAVE_DLG_FILE_TYPES());
    }
    if (fPath.length() > 0)
    {
        if (docs[curTabIdx].isReadOnly &&
            docs[curTabIdx].filePath == fPath)
        {
            QMessageBox::warning(nullptr, tr_values::SAVE_DLG_ERR_TITLE(),
                                 tr_values::SAVE_DLG_ERR_TEXT());
            return;
        }
        QFile file(fPath);
        if (file.open(QFile::WriteOnly))
        {
            QTextStream stream(&file);
            stream << tab->toHtml();

            dir = QFileInfo(fPath).absolutePath();
            settings->setValue(conf_param_name::DIR, dir);

            docs[curTabIdx].filePath = fPath;
            docs[curTabIdx].isReadOnly = false;
            docs[curTabIdx].isSaved = true;
        }
    }
    updateBasedOnReadOnlyState();
}

void MainWindow::updateBasedOnReadOnlyState(bool onlyCurrent)
{
    auto const readOnlySfx = tr_values::TAB_READ_ONLY_SUFFIX();
    QString title { };
    auto const start = onlyCurrent ? curTabIdx : 0;
    auto const end = onlyCurrent ? start + 1 : tabs->count();
    for (auto idx = start; idx < end; ++idx)
    {
        if (checkIndex(idx, docs))
        {
            auto const fPath = docs[idx].filePath;
            title = fPath.isEmpty() ? tr_values::TAB_DEF_TITLE()
                                    : QFileInfo(fPath).fileName()
                                      + (docs[idx].isReadOnly ? " " + readOnlySfx : " ");
            title = (docs[idx].isSaved ? "" : "*") + title;
            tabs->setTabText(idx, title);
            dynamic_cast<CustomTextEdit*>(tabs->currentWidget())->setReadOnly(docs[idx].isReadOnly);
        }
    }
}

void MainWindow::closeTab(int index, bool withoutSave)
{
    if (checkIndex(index, docs))
    {
        if (!docs[index].isReadOnly && !withoutSave && !docs[index].isSaved)
        {
            auto const reply = QMessageBox::question(nullptr,
                                                     tr_values::CLOSE_TAB_TITLE(),
                                                     tr_values::CLOSE_TAB_TEXT());
            if (reply == QMessageBox::Yes)
            {
                onSaveClicked();
            }
        }
        auto const tab = tabs->widget(index);
        tabs->removeTab(index);
        delete tab;

        docs[index].filePath.clear();
        docs[index].isReadOnly = false;
        docs.erase(std::next(docs.begin(), index));
    }
}

void MainWindow::initShortcuts()
{
    auto const openName = hotkeys_names::OPEN;
    auto const saveName = hotkeys_names::SAVE;
    auto const newName = hotkeys_names::_NEW;
    auto const quitName = hotkeys_names::QUIT;

    auto const openHotKey = settings->contains(openName)
            ? settings->value(openName, "").toString()
            : hotkeys_values::OPEN;
    auto const saveHotKey = settings->contains(saveName)
            ? settings->value(saveName, "").toString()
            : hotkeys_values::SAVE;
    auto const newHotKey = settings->contains(newName)
            ? settings->value(newName, "").toString()
            : hotkeys_values::_NEW;
    auto const quitHotKey = settings->contains(quitName)
            ? settings->value(quitName, "").toString()
            : hotkeys_values::QUIT;
    hotKeys[openName] = new QShortcut(QKeySequence(openHotKey), this);
    hotKeys[saveName] = new QShortcut(QKeySequence(saveHotKey), this);
    hotKeys[newName] = new QShortcut(QKeySequence(newHotKey), this);
    hotKeys[quitName] = new QShortcut(QKeySequence(quitHotKey), this);

    auto hotKey = hotKeys[openName]->key();
    emit updateLineEditPlaceholderText(openName, hotKey.toString(QKeySequence::NativeText));
    hotKey = hotKeys[saveName]->key();
    emit updateLineEditPlaceholderText(saveName, hotKey.toString(QKeySequence::NativeText));
    hotKey = hotKeys[newName]->key();
    emit updateLineEditPlaceholderText(newName, hotKey.toString(QKeySequence::NativeText));
    hotKey = hotKeys[quitName]->key();
    emit updateLineEditPlaceholderText(quitName, hotKey.toString(QKeySequence::NativeText));

    connect(hotKeys[openName], &QShortcut::activated, this, &MainWindow::onOpenClicked);
    connect(hotKeys[saveName], &QShortcut::activated, this, &MainWindow::onSaveClicked);
    connect(hotKeys[newName], &QShortcut::activated, this, &MainWindow::onNewClicked);
    connect(hotKeys[quitName], &QShortcut::activated, this, &MainWindow::onQuitClicked);
}

void MainWindow::initTabs()
{
    tabs = new QTabWidget(ui->centralwidget);
    tabs->setElideMode(Qt::ElideRight);
    tabs->setUsesScrollButtons(true);
    tabs->setTabsClosable(true);
    tabs->setMovable(true);
    ui->verticalLayout->addWidget(tabs);

    connect(tabs, &QTabWidget::tabCloseRequested, this, [this](auto idx) { closeTab(idx); });
    connect(tabs, &QTabWidget::currentChanged, this, &MainWindow::tabSelected);
    onNewClicked();
    connectActions();
    tabs->setFocus();
}

void MainWindow::connectButtons()
{
    connect(ui->new_button, &QPushButton::clicked, this, &MainWindow::onNewClicked);
    connect(ui->open_button, &QPushButton::clicked, this, &MainWindow::onOpenClicked);
    connect(ui->open_read_button, &QPushButton::clicked, this, &MainWindow::onOpenReadClicked);
    connect(ui->save_button, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    connect(ui->save_as_button, &QPushButton::clicked, this, &MainWindow::onSaveAsClicked);
    connect(ui->help_button, &QPushButton::clicked, this, &MainWindow::onHelpClicked);
    connect(ui->settings_button, &QPushButton::clicked, this, &MainWindow::onSettingsClicked);
    connect(ui->print_button, &QPushButton::clicked, this, &MainWindow::onPrintClicked);
    connect(ui->left_button, &QPushButton::clicked, this, &MainWindow::onLeftClicked);
    connect(ui->center_button, &QPushButton::clicked, this, &MainWindow::onCenterClicked);
    connect(ui->right_button, &QPushButton::clicked, this, &MainWindow::onRightClicked);
    connect(ui->font_button, &QPushButton::clicked, this, &MainWindow::onFontClicked);
    connect(ui->copy_format_button, &QPushButton::clicked, this, &MainWindow::onCopyFormatClicked);
    connect(ui->apply_format_button, &QPushButton::clicked, this, &MainWindow::onApplyFormatClicked);
    connect(ui->date_button, &QPushButton::clicked, this, &MainWindow::onDateClicked);
    connect(ui->time_button, &QPushButton::clicked, this, &MainWindow::onTimeClicked);
}

void MainWindow::connectActions()
{
    auto const tab = dynamic_cast<CustomTextEdit*>(tabs->currentWidget());
    connect(ui->copy_action, &QAction::triggered, tab, &CustomTextEdit::copyText);
    connect(ui->cut_action, &QAction::triggered, tab, &CustomTextEdit::cutText);
    connect(ui->paste_action, &QAction::triggered, tab, &CustomTextEdit::pasteText);
    connect(tab, &CustomTextEdit::textChanged, this, &MainWindow::onTextChanged);
    connect(ui->open_action, &QAction::triggered, this, &MainWindow::onOpenClicked);
    connect(ui->open_rd_action, &QAction::triggered, this, &MainWindow::onOpenReadClicked);
    connect(ui->save_action, &QAction::triggered, this, &MainWindow::onSaveClicked);
    connect(ui->help_action, &QAction::triggered, this, &MainWindow::onHelpClicked);
    connect(ui->print_action, &QAction::triggered, this, &MainWindow::onPrintClicked);
}

void MainWindow::prepareSettingsDialog()
{
    dir = settings->contains(conf_param_name::DIR)
            ? settings->value(conf_param_name::DIR, "").toString()
            : QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    lang = settings->contains(conf_param_name::LANG)
            ? settings->value(conf_param_name::LANG, "").toString()
            : lang_values::RU;
    theme = settings->contains(conf_param_name::THEME)
            ? settings->value(conf_param_name::THEME, "").toString()
            : theme_values::LIGHT;
    switchLanguage(lang);
    emit updateLanguageSelector(lang);
    switchTheme(theme);
    emit updateThemeSelector(theme);

    connect(settingsDialog, &SettingsDialog::englishSelected, this, &MainWindow::onEnglishSelected);
    connect(settingsDialog, &SettingsDialog::darkSelected, this, &MainWindow::onDarkSelected);
    connect(settingsDialog, &SettingsDialog::changeHotKey, this, &MainWindow::onChangeHotKey);
}
