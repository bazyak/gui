#include "finder_widget.h"
#include "ui_finder_widget.h"

#include <memory.h>

#include <QWidget>
#include <QTreeView>
#include <QFileSystemModel>

class FinderWidget::PrivateData
{
public:
    explicit PrivateData() { }
    ~PrivateData() { }

    Ui::FinderWidget ui_;
    QFileSystemModel model_;
};

FinderWidget::FinderWidget(QWidget* parent)
    : QWidget(parent)
    , data_(std::make_unique<PrivateData>())
{
    data_->ui_.setupUi(this);
    data_->model_.setRootPath("");
    data_->ui_.treeView->setModel(&data_->model_);

    for (int i = 1; i < data_->model_.columnCount(); ++i)
    {
        data_->ui_.treeView->hideColumn(i);
    }
    connect(data_->ui_.treeView, &QTreeView::clicked, this, &FinderWidget::elementClicked);
}

FinderWidget::~FinderWidget()
{
}

void FinderWidget::elementClicked(QModelIndex const& current)
{
    auto const dir = data_->model_.fileInfo(current).isDir()
            ? data_->model_.fileInfo(current).absoluteFilePath()
            : data_->model_.fileInfo(current).absolutePath();
    data_->ui_.current_dir_label->setText(dir);
    data_->ui_.current_dir_label->setToolTip(dir);
}
