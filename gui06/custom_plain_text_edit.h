#pragma once

#include <memory>

#include <QPlainTextEdit>
#include <QMenu>

class QContextMenuEvent;
class QWidget;

class CustomPlainTextEdit : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CustomPlainTextEdit(QWidget* parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;

public slots:
    void copyText();
    void cutText();
    void pasteText();

private:
    std::unique_ptr<QMenu> menu_;
};
