#pragma once

#include <QMainWindow>

#include <memory>

namespace Ui { class TasksWindow; }

class ProgressDelegate;
class TasksTableModel;
class TaskService;

class AllTasksWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AllTasksWindow(TasksTableModel& model, TaskService* parent);
    ~AllTasksWindow();

private:
    std::unique_ptr<Ui::TasksWindow> ui;
    ProgressDelegate* progressDelegate { nullptr };
};
