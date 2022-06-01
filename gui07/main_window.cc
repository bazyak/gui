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

#include <memory>

#include "key_event_filter.h"
#include "global_consts.h"
#include "finder_dialog.h"
#include "little_helpers.h"
#include "custom_plain_text_edit.h"
#include "settings_dialog.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui_(std::make_unique<Ui::MainWindow>())
    , settings_(std::make_unique<QSettings>(glob_values::CONF_FILE_NAME, QSettings::IniFormat))
    , finder_dialog_(std::make_unique<FinderDialog>(this))
    , settings_dialog_(std::make_unique<SettingsDialog>(this))
{
    ui_->setupUi(this);
    Q_INIT_RESOURCE(res);
    setFixedSize(width(), height());

    dir_ = settings_->contains(conf_param_name::DIR)
            ? settings_->value(conf_param_name::DIR, "").toString()
            : QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    initTabs();
    connectButtons();
    initShortcuts();
    initEventFilter();

    auto const lang = settings_->contains(conf_param_name::LANG)
            ? settings_->value(conf_param_name::LANG, "").toString()
            : lang_values::RU;
    switchLanguage(lang);
    settings_dialog_->setLanguageRadioButton(lang);

    auto const theme = settings_->contains(conf_param_name::THEME)
            ? settings_->value(conf_param_name::THEME, "").toString()
            : theme_values::LIGHT;
    switchTheme(theme);
    settings_dialog_->setThemeRadioButton(theme);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onOpenClicked()
{
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    if (!tab) return;

    auto const idx = tabs_->currentIndex();
    if (checkIndex(idx, docs_))
    {
        docs_[idx].is_read_only = false;
    }
    loadFile(tab);
    updateBasedOnReadOnlyState();
}

void MainWindow::onOpenReadClicked()
{
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    if (!tab) return;

    auto const idx = tabs_->currentIndex();
    if (checkIndex(idx, docs_))
    {
        docs_[idx].is_read_only = true;
    }
    loadFile(tab);
    updateBasedOnReadOnlyState();
}

void MainWindow::onSaveClicked()
{
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    if (!tab) return;

    auto const f_path = QFileDialog::getSaveFileName(this,
        tr_values::SAVE_DLG_TITLE(),
        dir_, tr_values::SAVE_DLG_FILE_TYPES());
    if (f_path.length() > 0)
    {
        if (docs_[tabs_->currentIndex()].is_read_only &&
            docs_[tabs_->currentIndex()].file_path == f_path)
        {
            QMessageBox::warning(nullptr, tr_values::SAVE_DLG_ERR_TITLE(),
                                 tr_values::SAVE_DLG_ERR_TEXT());
            return;
        }
        QFile file(f_path);
        if (file.open(QFile::WriteOnly))
        {
            QTextStream stream(&file);
            stream << tab->toHtml();

            dir_ = QFileInfo(f_path).absolutePath();
            settings_->setValue(conf_param_name::DIR, dir_);

            auto const idx = tabs_->currentIndex();
            if (checkIndex(idx, docs_))
            {
                docs_[idx].file_path = f_path;
                docs_[idx].is_read_only = false;
            }
        }
    }
    updateBasedOnReadOnlyState();
}

void MainWindow::onNewClicked()
{
    auto const idx = tabs_->addTab(new CustomPlainTextEdit(tabs_.get()), tr_values::TAB_DEF_TITLE());
    docs_.push_back({ });
    tabs_->setCurrentIndex(idx);
}

void MainWindow::onHelpClicked()
{
    auto const lang = settings_dialog_->getLanguage();

    QFile file(glob_values::HELP_FILE_PREFIX
               + QString(lang ? lang_values::EN : lang_values::RU)
               + glob_values::HELP_FILE_SUFFIX);
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
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    if (!tab) return;

    QPrinter printer;
    QPrintDialog dlg(&printer, this);
    dlg.setWindowTitle(tr_values::PRINT_DLG_TITLE());

    if (dlg.exec() != QDialog::Accepted) return;

    tab->print(&printer);
}

void MainWindow::onSettingsClicked()
{
    settings_dialog_->show();
}

void MainWindow::onFinderMenuClicked()
{
    finder_dialog_->exec();
}

void MainWindow::tabSelected(int index)
{
    if (index != current_tab_)
    {
        disconnect(ui_->copy_action, &QAction::triggered, 0, 0);
        disconnect(ui_->cut_action, &QAction::triggered, 0, 0);
        disconnect(ui_->paste_action, &QAction::triggered, 0, 0);

        if (index != -1)
        {
            auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
            connect(ui_->copy_action, &QAction::triggered, tab, &CustomPlainTextEdit::copyText);
            connect(ui_->cut_action, &QAction::triggered, tab, &CustomPlainTextEdit::cutText);
            connect(ui_->paste_action, &QAction::triggered, tab, &CustomPlainTextEdit::pasteText);
        }
        current_tab_ = index;
    }
}

void MainWindow::onLeftClicked()
{
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    if (!tab) return;

    tab->setAlignment(Qt::AlignLeft);
}

void MainWindow::onCenterClicked()
{
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    if (!tab) return;

    tab->setAlignment(Qt::AlignCenter);
}

void MainWindow::onRightClicked()
{
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    if (!tab) return;

    tab->setAlignment(Qt::AlignRight);
}

void MainWindow::onFontClicked()
{
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
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
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    if (!tab) return;

    if (tab->textCursor().hasSelection())
    {
        fmt_.fmt = tab->textCursor().charFormat();
        fmt_.align = tab->alignment();
    }
}

void MainWindow::onApplyFormatClicked()
{
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    if (!tab) return;

    if (!tab->textCursor().hasSelection())
    {
        tab->setCurrentCharFormat(fmt_.fmt);
    }
    else
    {
        tab->textCursor().setCharFormat(fmt_.fmt);
    }
    tab->setAlignment(fmt_.align);
}

void MainWindow::loadFile(CustomPlainTextEdit* tab)
{
    if (!tab) return;

    auto const f_path = QFileDialog::getOpenFileName(this,
        tr_values::OPEN_DLG_TITLE(),
        dir_, tr_values::OPEN_DLG_FILE_TYPES());
    if (f_path.length() > 0)
    {
        QFile file(f_path);
        if (file.open(QFile::ReadOnly | QFile::ExistingOnly))
        {
            QTextStream stream(&file);
            tab->setHtml(stream.readAll());

            dir_ = QFileInfo(f_path).absolutePath();
            settings_->setValue(conf_param_name::DIR, dir_);

            auto const idx = tabs_->currentIndex();
            if (checkIndex(idx, docs_))
            {
                docs_[idx].file_path = f_path;
            }
        }
    }
}

void MainWindow::updateBasedOnReadOnlyState()
{
    auto const read_only_sfx = tr_values::TAB_READ_ONLY_SUFFIX();
    QString title { };
    auto const idx = tabs_->currentIndex();
    if (checkIndex(idx, docs_))
    {
        auto const f_path = docs_[idx].file_path;
        title = f_path.isEmpty() ? tr_values::TAB_DEF_TITLE()
                                 : QFileInfo(f_path).fileName()
                                   + (docs_[idx].is_read_only ? " " + read_only_sfx : " ");
        tabs_->setTabText(idx, title);
        dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget())->setReadOnly(docs_[idx].is_read_only);
    }
}

void MainWindow::switchLanguage(QString const& lang)
{
    if (translator_.load(glob_values::TR_FILE_PREFIX + lang))
    {
        qApp->installTranslator(&translator_);
    }
    this->setWindowTitle(tr_values::WINDOW_TITLE());

    ui_->new_button->setText(tr_values::NEW_BTN());
    ui_->open_button->setText(tr_values::OPEN_BTN());
    ui_->open_read_button->setText(tr_values::OPEN_READ_BTN());
    ui_->save_button->setText(tr_values::SAVE_BTN());
    ui_->print_button->setText(tr_values::PRINT_BTN());
    ui_->settings_button->setText(tr_values::SETTINGS_BTN());
    ui_->help_button->setText(tr_values::HELP_BTN());

    ui_->file_menu->setTitle(tr_values::FILE_MENU());
    ui_->edit_menu->setTitle(tr_values::EDIT_MENU());
    ui_->open_action->setText(tr_values::OPEN_BTN());
    ui_->open_rd_action->setText(tr_values::OPEN_READ_BTN());
    ui_->save_action->setText(tr_values::SAVE_BTN());
    ui_->print_action->setText(tr_values::PRINT_BTN());
    ui_->help_action->setText(tr_values::HELP_BTN());
    ui_->copy_action->setText(tr_values::COPY_ACTION());
    ui_->cut_action->setText(tr_values::CUT_ACTION());
    ui_->paste_action->setText(tr_values::PASTE_ACTION());

    ui_->left_button->setText(tr_values::LEFT_BTN());
    ui_->center_button->setText(tr_values::CENTER_BTN());
    ui_->right_button->setText(tr_values::RIGHT_BTN());
    ui_->font_button->setText(tr_values::FONT_BTN());
    ui_->copy_format_button->setText(tr_values::COPY_FMT_BTN());
    ui_->apply_format_button->setText(tr_values::APPLY_FMT_BTN());

    ui_->task_5_3_menu->setTitle(tr_values::FINDER_MENU());
    ui_->finder_action->setText(tr_values::FINDER_ACTION());

    finder_dialog_->updateTranslatable();
    settings_dialog_->updateTranslatable();

    updateBasedOnReadOnlyState();
}

void MainWindow::switchTheme(QString const& theme)
{
    auto lst = ui_->centralwidget->findChildren<QWidget*>();
    lst.push_back(ui_->centralwidget);

    processStyleInList(lst, theme);

    finder_dialog_->switchTheme(theme);
    settings_dialog_->switchTheme(theme);
}

QSettings* MainWindow::getSettings()
{
    return settings_.get();
}

void MainWindow::closeTab(int index)
{
    if (checkIndex(index, docs_))
    {
        if (!docs_[index].is_read_only)
        {
            auto const reply = QMessageBox::question(nullptr, tr_values::CLOSE_TAB_TITLE(),
                tr_values::CLOSE_TAB_TEXT());
            if (reply == QMessageBox::Yes)
            {
                onSaveClicked();
            }
        }
        auto const tab = tabs_->widget(index);
        tabs_->removeTab(index);
        delete tab;

        docs_[index].file_path.clear();
        docs_[index].is_read_only = false;
        docs_.erase(std::next(docs_.begin(), index));
    }
}

void MainWindow::initShortcuts()
{
    auto open = settings_dialog_->getOpenEdit();
    auto save = settings_dialog_->getSaveEdit();
    auto _new = settings_dialog_->getNewEdit();
    auto quit = settings_dialog_->getQuitEdit();

    auto const open_hk = settings_->contains(open->objectName())
            ? settings_->value(open->objectName(), "").toString()
            : hotkeys_values::OPEN;
    auto const save_hk = settings_->contains(save->objectName())
            ? settings_->value(save->objectName(), "").toString()
            : hotkeys_values::SAVE;
    auto const new_hk = settings_->contains(_new->objectName())
            ? settings_->value(_new->objectName(), "").toString()
            : hotkeys_values::_NEW;
    auto const quit_hk = settings_->contains(quit->objectName())
            ? settings_->value(quit->objectName(), "").toString()
            : hotkeys_values::QUIT;
    hot_keys_[open->objectName()] = std::make_shared<QShortcut>(QKeySequence(open_hk), this);
    hot_keys_[save->objectName()] = std::make_shared<QShortcut>(QKeySequence(save_hk), this);
    hot_keys_[_new->objectName()] = std::make_shared<QShortcut>(QKeySequence(new_hk), this);
    hot_keys_[quit->objectName()] = std::make_shared<QShortcut>(QKeySequence(quit_hk), this);

    auto hot_key = hot_keys_[open->objectName()]->key();
    open->setPlaceholderText(hot_key.toString(QKeySequence::NativeText));
    hot_key = hot_keys_[save->objectName()]->key();
    save->setPlaceholderText(hot_key.toString(QKeySequence::NativeText));
    hot_key = hot_keys_[_new->objectName()]->key();
    _new->setPlaceholderText(hot_key.toString(QKeySequence::NativeText));
    hot_key = hot_keys_[quit->objectName()]->key();
    quit->setPlaceholderText(hot_key.toString(QKeySequence::NativeText));

    connect(hot_keys_[open->objectName()].get(), &QShortcut::activated, this, &MainWindow::onOpenClicked);
    connect(hot_keys_[save->objectName()].get(), &QShortcut::activated, this, &MainWindow::onSaveClicked);
    connect(hot_keys_[_new->objectName()].get(), &QShortcut::activated, this, &MainWindow::onNewClicked);
    connect(hot_keys_[quit->objectName()].get(), &QShortcut::activated, this, &MainWindow::onQuitClicked);
}

void MainWindow::initTabs()
{
    tabs_ = std::make_unique<QTabWidget>(ui_->centralwidget);
    tabs_->setElideMode(Qt::ElideRight);
    tabs_->setUsesScrollButtons(true);
    tabs_->setTabsClosable(true);
    tabs_->setMovable(true);
    ui_->verticalLayout->addWidget(tabs_.get());

    connect(tabs_.get(), &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(tabs_.get(), &QTabWidget::currentChanged, this, &MainWindow::tabSelected);
    onNewClicked();
    connectActions();
    tabs_->setFocus();
}

void MainWindow::connectButtons()
{
    connect(ui_->new_button, &QPushButton::clicked, this, &MainWindow::onNewClicked);
    connect(ui_->open_button, &QPushButton::clicked, this, &MainWindow::onOpenClicked);
    connect(ui_->open_read_button, &QPushButton::clicked, this, &MainWindow::onOpenReadClicked);
    connect(ui_->save_button, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    connect(ui_->help_button, &QPushButton::clicked, this, &MainWindow::onHelpClicked);
    connect(ui_->settings_button, &QPushButton::clicked, this, &MainWindow::onSettingsClicked);
    connect(ui_->print_button, &QPushButton::clicked, this, &MainWindow::onPrintClicked);
    connect(ui_->left_button, &QPushButton::clicked, this, &MainWindow::onLeftClicked);
    connect(ui_->center_button, &QPushButton::clicked, this, &MainWindow::onCenterClicked);
    connect(ui_->right_button, &QPushButton::clicked, this, &MainWindow::onRightClicked);
    connect(ui_->font_button, &QPushButton::clicked, this, &MainWindow::onFontClicked);
    connect(ui_->copy_format_button, &QPushButton::clicked, this, &MainWindow::onCopyFormatClicked);
    connect(ui_->apply_format_button, &QPushButton::clicked, this, &MainWindow::onApplyFormatClicked);
}

void MainWindow::connectActions()
{
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    connect(ui_->copy_action, &QAction::triggered, tab, &CustomPlainTextEdit::copyText);
    connect(ui_->cut_action, &QAction::triggered, tab, &CustomPlainTextEdit::cutText);
    connect(ui_->paste_action, &QAction::triggered, tab, &CustomPlainTextEdit::pasteText);
    connect(ui_->open_action, &QAction::triggered, this, &MainWindow::onOpenClicked);
    connect(ui_->open_rd_action, &QAction::triggered, this, &MainWindow::onOpenReadClicked);
    connect(ui_->save_action, &QAction::triggered, this, &MainWindow::onSaveClicked);
    connect(ui_->help_action, &QAction::triggered, this, &MainWindow::onHelpClicked);
    connect(ui_->print_action, &QAction::triggered, this, &MainWindow::onPrintClicked);
    connect(ui_->finder_action, &QAction::triggered, this, &MainWindow::onFinderMenuClicked);
}

void MainWindow::initEventFilter()
{
    ev_filter_ = std::make_shared<KeyEventFilter>(hot_keys_, this);
    settings_dialog_->getOpenEdit()->installEventFilter(ev_filter_.get());
    settings_dialog_->getSaveEdit()->installEventFilter(ev_filter_.get());
    settings_dialog_->getNewEdit()->installEventFilter(ev_filter_.get());
    settings_dialog_->getQuitEdit()->installEventFilter(ev_filter_.get());
}
