#include "main_window.h"
#include "ui_main_window.h"

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
#include <QKeySequence>
#include <QMetaEnum>
#include <QShortcut>
#include <QKeyCombination>
#include <QEvent>
#include <QKeySequence>
#include <memory>

#include "key_event_filter.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , is_read_only_(false)
    , ui_(std::make_unique<Ui::MainWindow>())
    , settings_(std::make_unique<QSettings>("settings.conf", QSettings::IniFormat))
{
    ui_->setupUi(this);
    Q_INIT_RESOURCE(res);
    setFixedSize(width(), height());

    dir_ = settings_->contains("dir")
            ? settings_->value("dir", "").toString()
            : QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    connect(ui_->open_button, &QPushButton::clicked, this, &MainWindow::on_open_clicked);
    connect(ui_->open_read_button, &QPushButton::clicked, this, &MainWindow::on_open_read_clicked);
    connect(ui_->save_button, &QPushButton::clicked, this, &MainWindow::on_save_clicked);
    connect(ui_->help_button, &QPushButton::clicked, this, &MainWindow::on_help_clicked);
    connect(ui_->english_radio_button, &QRadioButton::toggled, this, &MainWindow::on_english_selected);

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

    connect(hot_keys_[ui_->open_edit->objectName()].get(), &QShortcut::activated, this, &MainWindow::on_open_clicked);
    connect(hot_keys_[ui_->save_edit->objectName()].get(), &QShortcut::activated, this, &MainWindow::on_save_clicked);
    connect(hot_keys_[ui_->new_edit->objectName()].get(), &QShortcut::activated, this, &MainWindow::on_new_clicked);
    connect(hot_keys_[ui_->quit_edit->objectName()].get(), &QShortcut::activated, this, &MainWindow::on_quit_clicked);

    ev_filter_ = std::make_shared<KeyEventFilter>(hot_keys_);
    ui_->open_edit->installEventFilter(ev_filter_.get());
    ui_->save_edit->installEventFilter(ev_filter_.get());
    ui_->new_edit->installEventFilter(ev_filter_.get());
    ui_->quit_edit->installEventFilter(ev_filter_.get());
}

MainWindow::~MainWindow()
{
}


void MainWindow::on_open_clicked()
{
    is_read_only_ = false;
    load_file();
    update_based_on_read_only_state();
}

void MainWindow::on_open_read_clicked()
{
    is_read_only_ = true;
    load_file();
    update_based_on_read_only_state();
}

void MainWindow::on_save_clicked()
{
    auto const file_path = QFileDialog::getSaveFileName(this,
        tr("Выберите файл для сохранения"),
        dir_, tr("Текстовый файл (*.txt);;Любой файл (*.*)"));
    if (file_path.length() > 0)
    {
        if (is_read_only_ && file_path == file_path_)
        {
            QMessageBox::warning(this, tr("Ошибка"), tr("Этот файл открыт только для чтения"));
            return;
        }
        QFile file(file_path);
        if (file.open(QFile::WriteOnly))
        {
            QTextStream stream(&file);
            stream << ui_->plainTextEdit->toPlainText();
            file_path_ = file_path;
            dir_ = QFileInfo(file_path_).absolutePath();
            settings_->setValue("dir", dir_);
            is_read_only_ = false;
        }
    }
    update_based_on_read_only_state();
}

void MainWindow::on_new_clicked()
{
    file_path_.clear();
    is_read_only_ = false;
    ui_->plainTextEdit->clear();
    update_based_on_read_only_state();
}

void MainWindow::on_help_clicked()
{
    auto const lang = ui_->english_radio_button->isChecked();
    QFile file(":/help_" + QString(lang ? "en" : "ru") + ".txt");
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        auto const text = stream.readAll();
        QMessageBox::information(this, tr("Помощь"), text);
    }
}

void MainWindow::on_quit_clicked()
{
    qDebug() << "i am getting out.. ;)";
    qApp->exit();
}

void MainWindow::on_english_selected(bool checked)
{
    switch_language(checked ? "en" : "ru");
}

void MainWindow::on_dark_selected(bool checked)
{

}

void MainWindow::load_file()
{
    file_path_ = QFileDialog::getOpenFileName(this,
        tr("Выберите файл для открытия"),
        dir_, tr("Текстовые файлы (*.txt);;Все файлы (*.*)"));
    if (file_path_.length() > 0)
    {
        QFile file(file_path_);
        if (file.open(QFile::ReadOnly | QFile::ExistingOnly))
        {
            QTextStream stream(&file);
            ui_->plainTextEdit->setPlainText(stream.readAll());
            dir_ = QFileInfo(file_path_).absolutePath();
            settings_->setValue("dir", dir_);
        }
    }
}

void MainWindow::update_based_on_read_only_state()
{
    auto const rd = tr("[Только для чтения]");
    auto tit = tr("Урок 05");
    if (!file_path_.isEmpty())
    {
        tit = QFileInfo(file_path_).fileName()
                + (is_read_only_ ? " " + rd : " ")
                + " - " + tit;
    }
    this->setWindowTitle(tit);
    ui_->plainTextEdit->setReadOnly(is_read_only_);
}

void MainWindow::switch_language(QString language)
{
    if (translator_.load(":/gui_" + language))
    {
        qApp->installTranslator(&translator_);
    }
    ui_->settings_label->setText(tr("Настройки"));

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

    update_based_on_read_only_state();
}
