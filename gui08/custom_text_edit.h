#pragma once

#include <QTextEdit>

class QContextMenuEvent;
class QWidget;
class QMenu;

class CustomTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit CustomTextEdit(QWidget* parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;

public slots:
    void copyText();
    void cutText();
    void pasteText();

private:
    QMenu* menu { nullptr };
};
