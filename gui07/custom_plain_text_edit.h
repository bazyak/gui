#pragma once

#include <QPlainTextEdit>
#include <QMenu>

#include <memory>

class QContextMenuEvent;
class QWidget;

class CustomPlainTextEdit : public QTextEdit
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
