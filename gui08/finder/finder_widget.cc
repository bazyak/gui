#include "finder_widget.h"

#include <QObject>
#include <QWidget>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QTreeView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QStringList>
#include <QEvent>

#include <memory>

#include "seeker.h"
#include "user_event.h"

class FinderWidget::PrivateData
{
public:
    explicit PrivateData() { }
    ~PrivateData() { }

    int const kWidth = 700;
    int const kHeight = 600;

    QTreeView* fsTreeView { nullptr };
    QLineEdit* toFindEdit { nullptr };
    QPushButton* findBtn { nullptr };
    QPushButton* resetBtn { nullptr };
    QPushButton* nextBtn { nullptr };
    QPushButton* prevBtn { nullptr };
    QLabel* curDirLabel { nullptr };

    int curIdx = 0;
    QStringList found { };
    QFileSystemModel model { };
    Seeker* seeker { nullptr };
};

FinderWidget::FinderWidget(QWidget* parent)
    : QWidget(parent)
    , data(std::make_unique<PrivateData>())
{
    constructGui();

    data->model.setRootPath("");
    data->fsTreeView->setModel(&data->model);

    for (int i = 1; i < data->model.columnCount(); ++i)
    {
        data->fsTreeView->hideColumn(i);
    }
    connect(data->fsTreeView, &QTreeView::clicked, this, &FinderWidget::elementClicked);
    connect(data->toFindEdit, &QLineEdit::returnPressed, this, &FinderWidget::findFileSystemObject);
    connect(data->findBtn, &QPushButton::clicked, this, &FinderWidget::findFileSystemObject);
    connect(data->resetBtn, &QPushButton::clicked, this, &FinderWidget::resetSearch);
    connect(data->prevBtn, &QPushButton::clicked, this, &FinderWidget::onPrevFoundClicked);
    connect(data->nextBtn, &QPushButton::clicked, this, &FinderWidget::onNextFoundClicked);

    data->seeker = new Seeker(this);
    connect(data->seeker, &Seeker::resultReady, this, &FinderWidget::onSearchFinished);
}

FinderWidget::~FinderWidget()
{
}

int FinderWidget::getWidth() const
{
    return data->kWidth;
}

int FinderWidget::getHeight() const
{
    return data->kHeight;
}

void FinderWidget::customEvent(QEvent* event)
{
    if (event->type() == UserEvent::TypeEvent)
    {
        data->found.emplace_back(static_cast<UserEvent*>(event)->textMsg());
    }
}

void FinderWidget::elementClicked(QModelIndex const& current)
{
    printCurrentDir(current);
}

void FinderWidget::findFileSystemObject()
{
    auto const to_find = data->toFindEdit->text();
    if (!to_find.length()) return;

    if (!data->curDirLabel->text().length())
    {
        QMessageBox::warning(this, "Nowhere to find",
                             "Please select search directory");
        return;
    }
    resetFindContainer();
    disableAllCmdComps();
    enableResetBtn();

    data->seeker->setInputValues(to_find, data->curDirLabel->text());
    data->seeker->start();
}

void FinderWidget::onPrevFoundClicked()
{
    selectFoundResult(false);
}

void FinderWidget::onNextFoundClicked()
{
    selectFoundResult(true);
}

void FinderWidget::resetSearch()
{
    data->seeker->stopWork();
    for (; data->seeker->isRunning(); );
}

void FinderWidget::onSearchFinished(bool stopped)
{
    disableAllCmdComps();
    enableFindComps();
    if (stopped)
    {
        resetFindContainer();
        QMessageBox::warning(this, "Searching results", "Search reset");
        return;
    }
    if (data->found.isEmpty())
    {
        QMessageBox::warning(this, "Searching results",
                             "Search completed. Object not found");
        return;
    }
    QMessageBox::information(this, "Searching results",
                             "Search completed.\nResults are available by buttons \"<\" и \">\"");
    enablePrevNextBtns();
    onNextFoundClicked();
}

void FinderWidget::printCurrentDir(QModelIndex const& current)
{
    auto const dir = data->model.fileInfo(current).isDir()
            ? data->model.fileInfo(current).absoluteFilePath()
            : data->model.fileInfo(current).absolutePath();
    data->curDirLabel->setText(dir);
    data->curDirLabel->setToolTip(dir);
}

void FinderWidget::selectFoundResult(bool next)
{
    if (data->curIdx < data->found.size())
    {
        auto const last = data->found.size() - 1;
        if (next)
        {
            data->curIdx = data->curIdx != last ? data->curIdx + 1 : 0;
        }
        else
        {
            data->curIdx = !data->curIdx ? last : data->curIdx - 1;
        }
        auto const idx = data->model.index(data->found[data->curIdx]);
        data->fsTreeView->setCurrentIndex(idx);
        printCurrentDir(idx);
    }
}

void FinderWidget::resetFindContainer()
{
    data->found.clear();
    data->curIdx = 0;
}

void FinderWidget::enableResetBtn()
{
    data->resetBtn->setEnabled(true);
}

void FinderWidget::enableFindComps()
{
    data->findBtn->setEnabled(true);
    data->toFindEdit->setEnabled(true);
}

void FinderWidget::enablePrevNextBtns()
{
    data->prevBtn->setEnabled(true);
    data->nextBtn->setEnabled(true);
}

void FinderWidget::disableAllCmdComps()
{
    data->resetBtn->setEnabled(false);
    data->findBtn->setEnabled(false);
    data->nextBtn->setEnabled(false);
    data->prevBtn->setEnabled(false);
    data->toFindEdit->setEnabled(false);
}

void FinderWidget::constructGui()
{
    setFixedSize(data->kWidth, data->kHeight);

    auto const layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);

    data->curDirLabel = new QLabel(this);
    layout->addWidget(data->curDirLabel);

    data->fsTreeView = new QTreeView(this);
    data->fsTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    data->fsTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    data->fsTreeView->header()->setVisible(false);
    layout->addWidget(data->fsTreeView);

    auto const horizontal_layout = new QHBoxLayout(this);
    horizontal_layout->setContentsMargins(0, 0, 0, 0);

    data->toFindEdit = new QLineEdit(this);
    data->toFindEdit->setPlaceholderText("Enter file or folder name");
    horizontal_layout->addWidget(data->toFindEdit);

    data->findBtn = new QPushButton("Find", this);
    horizontal_layout->addWidget(data->findBtn);
    data->resetBtn = new QPushButton("Reset", this);
    horizontal_layout->addWidget(data->resetBtn);
    data->prevBtn = new QPushButton("<", this);
    horizontal_layout->addWidget(data->prevBtn);
    data->nextBtn = new QPushButton(">", this);
    horizontal_layout->addWidget(data->nextBtn);

    layout->addLayout(horizontal_layout);
    this->setLayout(layout);

    disableAllCmdComps();
    enableFindComps();
}
