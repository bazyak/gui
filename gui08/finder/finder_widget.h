#pragma once

#include <QWidget>

#include <memory>

class QModelIndex;
class QEvent;

class FinderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FinderWidget(QWidget* parent = nullptr);
    ~FinderWidget();
    int getWidth() const;
    int getHeight() const;

protected:
    void customEvent(QEvent* event) override;

private slots:
    void elementClicked(QModelIndex const& current);
    void findFileSystemObject();
    void onPrevFoundClicked();
    void onNextFoundClicked();
    void resetSearch();
    void onSearchFinished(bool stopped);

private:
    class PrivateData;

    void printCurrentDir(QModelIndex const& current);
    void selectFoundResult(bool next);
    void resetFindContainer();
    void enableResetBtn();
    void enableFindComps();
    void enablePrevNextBtns();
    void disableAllCmdComps();
    void constructGui();

    std::unique_ptr<PrivateData> data;
};
