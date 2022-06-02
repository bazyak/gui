#include "finder_widget.h"

#include <QWidget>
#include <QModelIndex>
#include <QTreeView>
#include <QFileSystemModel>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDirIterator>
#include <QMessageBox>

#include <memory>

class FinderWidget::PrivateData
{
public:
    explicit PrivateData()
    {
        layout_ = std::make_unique<QVBoxLayout>();
        layout_->setContentsMargins(10, 10, 10, 10);

        current_dir_label_ = std::make_unique<QLabel>();
        layout_->addWidget(current_dir_label_.get());

        tree_view_ = std::make_unique<QTreeView>();
        tree_view_->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tree_view_->setSelectionMode(QAbstractItemView::SingleSelection);
        tree_view_->header()->setVisible(false);
        layout_->addWidget(tree_view_.get());

        horizontal_layout_ = std::make_unique<QHBoxLayout>();
        horizontal_layout_->setContentsMargins(0, 0, 0, 0);

        to_find_ = std::make_unique<QLineEdit>();
        to_find_->setPlaceholderText("Введите имя папки или файла");
        horizontal_layout_->addWidget(to_find_.get());

        find_btn_ = std::make_unique<QPushButton>();
        find_btn_->setText("Найти");
        horizontal_layout_->addWidget(find_btn_.get());

        layout_->addLayout(horizontal_layout_.get());
    }
    ~PrivateData() { }

    int const kWidth = 700;
    int const kHeight = 600;

    std::unique_ptr<QVBoxLayout> layout_;
    std::unique_ptr<QLabel> current_dir_label_;
    std::unique_ptr<QTreeView> tree_view_;
    std::unique_ptr<QLineEdit> to_find_;
    std::unique_ptr<QPushButton> find_btn_;
    std::unique_ptr<QHBoxLayout> horizontal_layout_;
    QFileSystemModel model_;
};

FinderWidget::FinderWidget(QWidget* parent)
    : QWidget(parent)
    , data_(std::make_unique<PrivateData>())
{
    setFixedSize(data_->kWidth, data_->kHeight);
    setWindowModality(Qt::NonModal);

    this->setLayout(data_->layout_.get());

    data_->model_.setRootPath("");
    data_->tree_view_->setModel(&data_->model_);

    for (int i = 1; i < data_->model_.columnCount(); ++i)
    {
        data_->tree_view_->hideColumn(i);
    }
    connect(data_->tree_view_.get(), &QTreeView::clicked, this, &FinderWidget::elementClicked);
    connect(data_->find_btn_.get(), &QPushButton::clicked, this, &FinderWidget::onFindBtnClicked);
}

FinderWidget::~FinderWidget()
{
}

int FinderWidget::getWidth() const
{
    return data_->kWidth;
}

int FinderWidget::getHeight() const
{
    return data_->kHeight;
}

void FinderWidget::elementClicked(QModelIndex const& current)
{
    auto const dir = data_->model_.fileInfo(current).isDir()
            ? data_->model_.fileInfo(current).absoluteFilePath()
            : data_->model_.fileInfo(current).absolutePath();
    data_->current_dir_label_->setText(dir);
    data_->current_dir_label_->setToolTip(dir);
}

void FinderWidget::onFindBtnClicked()
{
    auto const to_find = data_->to_find_->text();
    if (!to_find.length()) return;

    if (!data_->current_dir_label_->text().length())
    {
        QMessageBox::warning(this, "Nowhere to find", "Nowhere to find. Please select search directory");
    }

    QDirIterator it(data_->current_dir_label_->text(), QDir::NoFilter, QDirIterator::Subdirectories);
    auto flag = false;
    while (it.hasNext())
    {
        auto const el = QFileInfo(it.next());
        auto name = el.isFile() ? el.fileName()
                                     : el.dir().dirName();
        if (name == to_find)
        {
            flag = true;
            QMessageBox::information(this, "", name);
            break;
        }
    }
    if (!flag)
    {
        QMessageBox::information(this, "Результаты поиска", "Объект не найден");
    }
}
