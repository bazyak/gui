#include "main_window.h"
#include "ui_main_window.h"

#include <QPushButton>

#include <memory>

#include "finder_widget.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui_(std::make_unique<Ui::MainWindow>())
{
    ui_->setupUi(this);
    finder_ = std::make_unique<FinderWidget>(ui_->centralwidget);

    setFixedSize(finder_->getWidth(), finder_->getHeight() + 35);

    connect(ui_->exit_button, &QPushButton::clicked, this, &MainWindow::onExitClicked);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onExitClicked()
{
    close();
}
