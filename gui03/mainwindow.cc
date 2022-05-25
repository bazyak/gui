#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include <QResource>

#include "html_parser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
    , parser_(std::make_unique<HtmlParser>())
{
    ui_->setupUi(this);
    Q_INIT_RESOURCE(res);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::on_ex1_open_button_clicked()
{
    auto const file_name = QFileDialog::getOpenFileName(this, "Выберите файл для открытия",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "Текстовые файлы (*.txt);;Все файлы (*.*)");
    if (file_name.length() > 0)
    {
        QFile file(file_name);
        if (file.open(QFile::ReadOnly | QFile::ExistingOnly))
        {
            QTextStream stream(&file);
            ui_->plainTextEdit->setPlainText(stream.readAll());
        }
    }
}

void MainWindow::on_ex1_save_button_clicked()
{
    auto const file_name = QFileDialog::getSaveFileName(this, "Выберите файл для сохранения",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "Текстовый файл (*.txt);;Любой файл (*.*)");
    if (file_name.length() > 0)
    {
        QFile file(file_name);
        if (file.open(QFile::WriteOnly))
        {
            QTextStream stream(&file);
            stream << ui_->plainTextEdit->toPlainText();
        }
    }
}

void MainWindow::on_ex1_help_button_clicked()
{
    QFile file(":/help.txt");
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        auto const text = stream.readAll();
        QMessageBox::information(this, "Help", text);
    }
}

void MainWindow::on_ex2_open_button_clicked()
{
    auto const file_name = QFileDialog::getOpenFileName(this, "Выберите файл для открытия",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "HTML файлы (*.html);;Все файлы (*.*)");
    if (file_name.length() > 0)
    {
        QFile file(file_name);
        if (file.open(QFile::ReadOnly | QFile::ExistingOnly))
        {
            QTextStream stream(&file);
            auto const text = stream.readAll();
            auto res = parser_->Parse(text);

            auto const city = QString::fromStdString(res["city"]);
            auto const temperature = QString::fromStdString(res["temp"])
                    + ", ощущается как " + QString::fromStdString(res["feel"]);
            auto const description = QString::fromStdString(res["desc"]);
            auto const humidity = "Влажность " + QString::fromStdString(res["humi"]);
            ui_->weather_plain_text->clear();
            ui_->weather_plain_text->appendPlainText(city);
            ui_->weather_plain_text->appendPlainText(temperature);
            ui_->weather_plain_text->appendPlainText(description);
            ui_->weather_plain_text->appendPlainText(humidity);

            auto const usd = "USD: " + QString::fromStdString(res["usd"])
                    + ", изменился на " + QString::fromStdString(res["usd_change"]);
            auto const eur = "EUR: " + QString::fromStdString(res["eur"])
                    + ", изменился на " + QString::fromStdString(res["eur_change"]);
            ui_->rates_plain_text->clear();
            ui_->rates_plain_text->appendPlainText(usd);
            ui_->rates_plain_text->appendPlainText(eur);
        }
    }
}
