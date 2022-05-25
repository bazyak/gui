#include "custom_plain_text_edit.h"

#include <memory>
#include <QApplication>
#include <QClipboard>
#include <QAction>
#include <QMenu>

CustomPlainTextEdit::CustomPlainTextEdit(QWidget* parent)
    : QPlainTextEdit(parent)
{
    menu_ = std::make_unique<QMenu>(this);
    QAction* copyAction = menu_->addAction(tr("Копировать"));
    QAction* cutAction = menu_->addAction(tr("Вырезать"));
    QAction* pasteAction = menu_->addAction(tr("Вставить"));
    connect(copyAction, &QAction::triggered, this, &CustomPlainTextEdit::copyText);
    connect(cutAction, &QAction::triggered, this, &CustomPlainTextEdit::cutText);
    connect(pasteAction, &QAction::triggered, this, &CustomPlainTextEdit::pasteText);
}

void CustomPlainTextEdit::contextMenuEvent(QContextMenuEvent* event)
{
    menu_->exec(event->globalPos());
}

void CustomPlainTextEdit::copyText()
{
    auto str = this->textCursor().selectedText();
    qApp->clipboard()->setText(str);
}

void CustomPlainTextEdit::cutText()
{
    auto str = this->textCursor().selectedText();
    this->textCursor().removeSelectedText();
    qApp->clipboard()->setText(str);
}

void CustomPlainTextEdit::pasteText()
{
    QString str = qApp->clipboard()->text();
    this->textCursor().insertText(str);
}
