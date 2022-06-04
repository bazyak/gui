#include "main_window.h"
#include "ui_main_window.h"

#include <QPushButton>

#include <memory>

#include "finder_widget.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::MainWindow>())
{
    ui->setupUi(this);
    finder = std::make_unique<FinderWidget>(ui->centralwidget);

    setFixedSize(finder->getWidth(), finder->getHeight() + 35);
    setWindowModality(Qt::NonModal);

    connect(ui->exit_button, &QPushButton::clicked, this, &MainWindow::onExitClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onExitClicked()
{
    close();
}
