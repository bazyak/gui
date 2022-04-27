#pragma once

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndex>
#include <QIcon>
#include <QDebug>
#include <memory>

#include "parse_text.h"
#include "lang_model.h"

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
    // ex 1
    void on_plainTextEdit_textChanged();

    // ex 2
    void on_ex2_print_button_clicked();
    void on_ex2_add_button_clicked();
    void on_ex2_remove_button_clicked();
    void on_ex2_view_mode_stateChanged(int arg1);

    // ex 3
    void on_ex3_color_button_clicked();
    void on_ex3_clear_color_button_clicked();

private:
    Ui::MainWindow* ui_;

    std::unique_ptr<ParseText> parse_text_;
    LangModel model_;
};
