#include "all_tasks_window.h"
#include "ui_all_tasks_window.h"

#include <QPushButton>

#include <memory>

#include "progress_delegate.h"
#include "tasks_table_model.h"
#include "task_service.h"

AllTasksWindow::AllTasksWindow(TasksTableModel& model, TaskService* parent)
    : QMainWindow(nullptr)
    , ui(std::make_unique<Ui::TasksWindow>())
{
    ui->setupUi(this);

    ui->tasksTableView->setModel(&model);

    progressDelegate = new ProgressDelegate(ui->tasksTableView);

    ui->tasksTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tasksTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tasksTableView->verticalHeader()->setVisible(false);
    ui->tasksTableView->setItemDelegateForColumn(3, progressDelegate);

    connect(parent, &TaskService::initEnd, this,
            [this](auto succeed, auto count)
            {
                Q_UNUSED(count)
                if (succeed)
                {
                    ui->tasksTableView->setColumnHidden(0, true);
                    ui->tasksTableView->setColumnWidth(1, 300);
                    ui->tasksTableView->setColumnWidth(2, 90);
                    ui->tasksTableView->setColumnWidth(3, 110);
                }
            }
    );
    connect(ui->closeButton, &QPushButton::clicked, this, [this]() { close(); });
}

AllTasksWindow::~AllTasksWindow()
{
}
