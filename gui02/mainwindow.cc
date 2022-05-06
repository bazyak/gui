#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QStringList>
#include <QMessageBox>
#include <memory>
#include <unordered_set>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
    , parse_text_(std::make_unique<ParseText>())
    , model_(this)
{
    ui_->setupUi(this);
    // ex 1
    if (!parse_text_) close();

    // ex 2
    ui_->listView->setModel(model_.getModel());

    ui_->listView->setDragDropOverwriteMode(false);
    ui_->listView->setDragEnabled(true);
    ui_->listView->setDragDropMode(QAbstractItemView::InternalMove);

    // ex 3
    ui_->tableWidget->horizontalHeader()->resizeSection(0, 30);
    ui_->tableWidget->horizontalHeader()->resizeSection(1, 110);
    ui_->tableWidget->horizontalHeader()->resizeSection(2, 110);
    ui_->tableWidget->horizontalHeader()->resizeSection(3, 145);
    ui_->tableWidget->horizontalHeader()->setSectionsMovable(false);
    ui_->tableWidget->horizontalHeader()->setSectionsClickable(false);
    ui_->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    for (int row = 0; row < ui_->tableWidget->rowCount(); ++row)
    {
        for (int col = 0; col < ui_->tableWidget->columnCount(); ++col)
        {
            if (!ui_->tableWidget->item(row, col))
            {
                ui_->tableWidget->setItem(row, col, new QTableWidgetItem);
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui_;
}

// ex 1
void MainWindow::on_plainTextEdit_textChanged()
{
    auto text = ui_->plainTextEdit->toPlainText();
    auto const to_replace = parse_text_->toReplace(text);
    if (!to_replace.empty())
    {
        auto const cursor = ui_->plainTextEdit->textCursor();
        parse_text_->parse(text, to_replace);
        ui_->plainTextEdit->setPlainText(text);
        ui_->plainTextEdit->setTextCursor(cursor);
    }
}

// ex 2
void MainWindow::on_ex2_print_button_clicked()
{
    QString str { };
    for (int row = 0; row < model_.count(); ++row)
    {
        str += model_.getValueByIndex(row) + "\n";
    }
    QMessageBox::information(this, "Info", str);
}

void MainWindow::on_ex2_add_button_clicked()
{
    model_.addItem();
    ui_->listView->setCurrentIndex(model_.getLastIndex());
    ui_->listView->edit(model_.getLastIndex());
}

void MainWindow::on_ex2_remove_button_clicked()
{
    auto const selection = ui_->listView->selectionModel()->selection();
    if (!selection.empty())
    {
        auto const idx = selection.indexes()[0].row();
        model_.removeItemByIndex(idx);
    }
}

void MainWindow::on_ex2_view_mode_stateChanged(int arg1)
{
    if (arg1)
    {
        ui_->listView->setViewMode(QListView::IconMode);
        ui_->listView->setMovement(QListView::Snap);
        return;
    }
    ui_->listView->setViewMode(QListView::ListMode);
}

// ex 3
void MainWindow::on_ex3_color_button_clicked()
{
    std::unordered_set<int> rows;
    auto const sel = ui_->tableWidget->selectedItems();
    std::for_each(sel.begin(), sel.end(), [&rows](auto const& el) { rows.insert(el->row()); });

    for (auto const& row : rows)
    {
        for (int col = 0; col < ui_->tableWidget->columnCount(); ++col)
        {
            ui_->tableWidget->item(row, col)->setBackground(Qt::lightGray);
        }
    }
}

void MainWindow::on_ex3_clear_color_button_clicked()
{
    for (int row = 0; row < ui_->tableWidget->rowCount(); ++row)
    {
        for (int col = 0; col < ui_->tableWidget->columnCount(); ++col)
        {
            ui_->tableWidget->item(row, col)->setBackground(Qt::white);
        }
    }
}
