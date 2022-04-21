#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_ex1_calc_button_clicked();

    void on_ex2_calc_button_clicked();

    void on_ex3_add_button_clicked();

    void on_ex3_replace_button_clicked();

    void on_ex3_html_button_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
