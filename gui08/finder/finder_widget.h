#pragma once

#include <QWidget>

#include <memory>

class QModelIndex;

class FinderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FinderWidget(QWidget* parent = nullptr);
    ~FinderWidget();
    int getWidth() const;
    int getHeight() const;

private slots:
    void elementClicked(QModelIndex const& current);
    void onFindBtnClicked();

private:
    class PrivateData;
    std::unique_ptr<PrivateData> data_;
};
