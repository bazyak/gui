#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Exercise 1
void MainWindow::on_ex1_calc_button_clicked()
{
    auto a = ui->ex1_edit_a->text().toDouble();
    auto b = ui->ex1_edit_b->text().toDouble();
    auto d = ui->ex1_edit_d->text().toDouble();
    auto c = ui->ex1_edit_c->text().toDouble() - d;

    QString res = "";
    if (!a)
    {
        res = !b ? "неизвестных нет"
                 : "x = " + QString::number(-c / b);
    }
    else
    {
        auto dis = b*b - 4*a*c;

        if (dis < 0) res = "корней нет";
        else
        {
            auto den = 2*a;

            res = !dis ? "x = "+ QString::number(-b / den)
                       : "x1 = " + QString::number((-b - qSqrt(dis)) / den) + ",  " +
                         "x2 = " + QString::number((-b + qSqrt(dis)) / den);
        }
    }
    ui->ex1_label_result->setText(res);
}

void MainWindow::on_ex2_calc_button_clicked()
{
    auto a = ui->ex2_edit_a->text().toDouble();
    auto b = ui->ex2_edit_b->text().toDouble();
    auto angle = ui->ex2_edit_angle->text().toDouble();

    if (ui->ex2_select_deg->isChecked())
    {
        angle = qDegreesToRadians(angle);
    }
    auto c = qSqrt(a*a + b*b - 2*a*b*qCos(angle));

    ui->ex2_label_result->setText(QString::number(c));
}


void MainWindow::on_ex3_add_button_clicked()
{
    auto text = ui->ex3_plain_text_1->toPlainText();
    ui->ex3_plain_text_2->appendPlainText(text);
}


void MainWindow::on_ex3_replace_button_clicked()
{
    auto text = ui->ex3_plain_text_2->toPlainText();
    ui->ex3_plain_text_1->setPlainText(text);
}



void MainWindow::on_ex3_html_button_clicked()
{
    static QString const text = "<font color='red'><b>This is only a test</b></font>";
    ui->ex3_plain_text_2->appendHtml(text);
}

