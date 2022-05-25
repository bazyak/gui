#include "main_window.h"
#include "ui_main_window.h"

#include <memory>

#include <QDebug>
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

#include "key_event_filter.h"
#include "global_consts.h"
#include "finder_dialog.h"
#include "little_helpers.h"
#include "custom_plain_text_edit.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui_(std::make_unique<Ui::MainWindow>())
    , settings_(std::make_unique<QSettings>("settings.conf", QSettings::IniFormat))
    , finder_dialog_(std::make_unique<FinderDialog>(this))
{
    ui_->setupUi(this);
    Q_INIT_RESOURCE(res);
    setFixedSize(width(), height());

    tabs_ = std::make_unique<QTabWidget>(ui_->centralwidget);
    tabs_->setGeometry(QRect(30, 30, 301, 211));
    tabs_->setElideMode(Qt::ElideRight);
    tabs_->setUsesScrollButtons(true);
    tabs_->setTabsClosable(true);
    tabs_->setMovable(true);

    connect(tabs_.get(), &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
    connect(tabs_.get(), &QTabWidget::currentChanged, this, &MainWindow::tabSelected);
    onNewClicked();

    dir_ = settings_->contains("dir")
            ? settings_->value("dir", "").toString()
            : QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    connect(ui_->new_button, &QPushButton::clicked, this, &MainWindow::onNewClicked);
    connect(ui_->open_button, &QPushButton::clicked, this, &MainWindow::onOpenClicked);
    connect(ui_->open_read_button, &QPushButton::clicked, this, &MainWindow::onOpenReadClicked);
    connect(ui_->save_button, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
    connect(ui_->help_button, &QPushButton::clicked, this, &MainWindow::onHelpClicked);
    connect(ui_->english_radio_button, &QRadioButton::toggled, this, &MainWindow::onEnglishSelected);
    connect(ui_->dark_radio_button, &QRadioButton::toggled, this, &MainWindow::onDarkSelected);
    connect(ui_->print_button, &QPushButton::clicked, this, &MainWindow::onPrintClicked);

    hot_keys_[ui_->open_edit->objectName()] = std::make_shared<QShortcut>(QKeySequence("Ctrl+O"), this);
    hot_keys_[ui_->save_edit->objectName()] = std::make_shared<QShortcut>(QKeySequence("Ctrl+S"), this);
    hot_keys_[ui_->new_edit->objectName()] = std::make_shared<QShortcut>(QKeySequence("Ctrl+N"), this);
    hot_keys_[ui_->quit_edit->objectName()] = std::make_shared<QShortcut>(QKeySequence("Ctrl+Q"), this);

    auto hot_key = hot_keys_[ui_->open_edit->objectName()]->key();
    ui_->open_edit->setPlaceholderText(hot_key.toString(QKeySequence::NativeText));
    hot_key = hot_keys_[ui_->save_edit->objectName()]->key();
    ui_->save_edit->setPlaceholderText(hot_key.toString(QKeySequence::NativeText));
    hot_key = hot_keys_[ui_->new_edit->objectName()]->key();
    ui_->new_edit->setPlaceholderText(hot_key.toString(QKeySequence::NativeText));
    hot_key = hot_keys_[ui_->quit_edit->objectName()]->key();
    ui_->quit_edit->setPlaceholderText(hot_key.toString(QKeySequence::NativeText));

    connect(hot_keys_[ui_->open_edit->objectName()].get(), &QShortcut::activated, this, &MainWindow::onOpenClicked);
    connect(hot_keys_[ui_->save_edit->objectName()].get(), &QShortcut::activated, this, &MainWindow::onSaveClicked);
    connect(hot_keys_[ui_->new_edit->objectName()].get(), &QShortcut::activated, this, &MainWindow::onNewClicked);
    connect(hot_keys_[ui_->quit_edit->objectName()].get(), &QShortcut::activated, this, &MainWindow::onQuitClicked);

    connect(ui_->finder_action, &QAction::triggered, this, &MainWindow::onFinderMenuClicked);

    ev_filter_ = std::make_shared<KeyEventFilter>(hot_keys_);
    ui_->open_edit->installEventFilter(ev_filter_.get());
    ui_->save_edit->installEventFilter(ev_filter_.get());
    ui_->new_edit->installEventFilter(ev_filter_.get());
    ui_->quit_edit->installEventFilter(ev_filter_.get());

    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    connect(ui_->copy_action, &QAction::triggered, tab, &CustomPlainTextEdit::copyText);
    connect(ui_->cut_action, &QAction::triggered, tab, &CustomPlainTextEdit::cutText);
    connect(ui_->paste_action, &QAction::triggered, tab, &CustomPlainTextEdit::pasteText);
    connect(ui_->open_action, &QAction::triggered, this, &MainWindow::onOpenClicked);
    connect(ui_->open_rd_action, &QAction::triggered, this, &MainWindow::onOpenReadClicked);
    connect(ui_->save_action, &QAction::triggered, this, &MainWindow::onSaveClicked);
    connect(ui_->help_action, &QAction::triggered, this, &MainWindow::onHelpClicked);
    connect(ui_->print_action, &QAction::triggered, this, &MainWindow::onPrintClicked);

    tabs_->setFocus();
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
    loadFile();
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
    loadFile();
    updateBasedOnReadOnlyState();
}

void MainWindow::onSaveClicked()
{
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    if (!tab) return;

    auto const f_path = QFileDialog::getSaveFileName(this,
        tr("Выберите файл для сохранения"),
        dir_, tr("Текстовый файл (*.txt);;Любой файл (*.*)"));
    if (f_path.length() > 0)
    {
        if (docs_[tabs_->currentIndex()].is_read_only &&
            docs_[tabs_->currentIndex()].file_path == f_path)
        {
            QMessageBox::warning(nullptr, tr("Ошибка"), tr("Этот файл открыт только для чтения"));
            return;
        }
        QFile file(f_path);
        if (file.open(QFile::WriteOnly))
        {
            QTextStream stream(&file);
            stream << tab->toPlainText();
            dir_ = QFileInfo(f_path).absolutePath();
            settings_->setValue("dir", dir_);
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
    auto const idx = tabs_->addTab(new CustomPlainTextEdit(tabs_.get()), tr("без имени"));
    docs_.push_back({ });
    tabs_->setCurrentIndex(idx);
}

void MainWindow::onHelpClicked()
{
    auto const lang = ui_->english_radio_button->isChecked();
    QFile file(":/help_" + QString(lang ? "en" : "ru") + ".txt");
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        auto const text = stream.readAll();
        QMessageBox::information(nullptr, tr("Помощь"), text);
    }
}

void MainWindow::onQuitClicked()
{
    qDebug() << "i am getting out.. ;)";
    qApp->exit();
}

void MainWindow::onPrintClicked()
{
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    if (!tab) return;

    QPrinter printer;
    QPrintDialog dlg(&printer, this);
    dlg.setWindowTitle(tr("Печать"));

    if (dlg.exec() != QDialog::Accepted) return;

    tab->print(&printer);
}

void MainWindow::onEnglishSelected(bool checked)
{
    switchLanguage(checked ? "en" : "ru");
}

void MainWindow::onDarkSelected(bool checked)
{
    switchTheme(checked ? theme_values::DARK : theme_values::LIGHT);
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

void MainWindow::loadFile()
{
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    if (!tab) return;

    auto const f_path = QFileDialog::getOpenFileName(this,
        tr("Выберите файл для открытия"),
        dir_, tr("Текстовые файлы (*.txt);;Все файлы (*.*)"));
    if (f_path.length() > 0)
    {
        QFile file(f_path);
        if (file.open(QFile::ReadOnly | QFile::ExistingOnly))
        {
            QTextStream stream(&file);
            tab->setPlainText(stream.readAll());
            dir_ = QFileInfo(f_path).absolutePath();
            settings_->setValue("dir", dir_);
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
    auto const tab = dynamic_cast<CustomPlainTextEdit*>(tabs_->currentWidget());
    if (!tab) return;

    auto const rd = tr("[Только для чтения]");
    QString title { };
    auto const idx = tabs_->currentIndex();    
    if (checkIndex(idx, docs_))
    {
        auto const f_path = docs_[idx].file_path;
        title = f_path.isEmpty() ? tr("без имени")
                                 : QFileInfo(f_path).fileName()
                                   + (docs_[idx].is_read_only ? " " + rd : " ");
        tabs_->setTabText(idx, title);
        tab->setReadOnly(docs_[idx].is_read_only);
    }
}

void MainWindow::switchLanguage(QString const& language)
{
    if (translator_.load(":/gui_" + language))
    {
        qApp->installTranslator(&translator_);
    }
    this->setWindowTitle(tr("Урок 06"));
    ui_->settings_label->setText(tr("Настройки"));

    ui_->new_button->setText(tr("Новый"));
    ui_->open_button->setText(tr("Открыть"));
    ui_->open_read_button->setText(tr("Открыть на чтение"));
    ui_->save_button->setText(tr("Сохранить"));
    ui_->help_button->setText(tr("Справка"));

    ui_->hotkeys_group->setTitle(tr("Горячие клавиши"));
    ui_->lang_group->setTitle(tr("Язык"));
    ui_->theme_group->setTitle(tr("Оформление"));
    ui_->light_radio_button->setText(tr("Светлое"));
    ui_->dark_radio_button->setText(tr("Тёмное"));

    ui_->label_open->setText(tr("Открыть"));
    ui_->label_save->setText(tr("Сохранить"));
    ui_->label_new->setText(tr("Новый"));
    ui_->label_quit->setText(tr("Выход"));

    ui_->task_5_3_menu->setTitle(tr("Задание 5.3"));
    ui_->finder_action->setText(tr("Просмотрщик"));
    ui_->file_menu->setTitle(tr("Файл"));
    ui_->edit_menu->setTitle(tr("Правка"));
    ui_->open_action->setText(tr("Открыть"));
    ui_->open_rd_action->setText(tr("Открыть на чтение"));
    ui_->save_action->setText(tr("Сохранить"));
    ui_->print_action->setText(tr("Печать"));
    ui_->help_action->setText(tr("Справка"));
    ui_->copy_action->setText(tr("Копировать"));
    ui_->cut_action->setText(tr("Вырезать"));
    ui_->paste_action->setText(tr("Вставить"));

    finder_dialog_->updateTranslatable();

    updateBasedOnReadOnlyState();
}

void MainWindow::switchTheme(QString const& theme)
{
    auto lst = ui_->centralwidget->findChildren<QWidget*>();
    lst.push_back(ui_->centralwidget);

    processStyleInList(lst, theme);

    finder_dialog_->switchTheme(theme);
}

void MainWindow::closeTab(int index)
{  
    if (checkIndex(index, docs_))
    {
        if (!docs_[index].is_read_only)
        {
            auto const reply = QMessageBox::question(nullptr, tr("Сохранить?"),
                tr("Сохранить файл перед закрытием?"));
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
