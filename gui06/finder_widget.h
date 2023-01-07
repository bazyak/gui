#pragma once

#include <memory>

#include <QWidget>
#include <QModelIndex>

namespace Ui { class FinderWidget; }

class FinderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FinderWidget(QWidget* parent = nullptr);
    ~FinderWidget();

private slots:
    void elementClicked(QModelIndex const& current);

private:
    class PrivateData;
    std::unique_ptr<PrivateData> data_;
};
