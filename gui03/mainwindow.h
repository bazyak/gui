#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

#include "html_parser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_ex1_open_button_clicked();
    void on_ex1_save_button_clicked();
    void on_ex1_help_button_clicked();

    void on_ex2_open_button_clicked();

private:
    Ui::MainWindow *ui_;
    std::unique_ptr<HtmlParser> parser_;
};
#endif // MAINWINDOW_H
