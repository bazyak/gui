#include "custom_plain_text_edit.h"

#include <QApplication>
#include <QClipboard>
#include <QAction>
#include <QMenu>

#include <memory>

#include "global_consts.h"

CustomPlainTextEdit::CustomPlainTextEdit(QWidget* parent)
    : QTextEdit(parent)
{
    menu_ = std::make_unique<QMenu>(this);
    setViewportMargins(0, 0, 15, 0);
    QAction* copyAction = menu_->addAction(tr_values::COPY_ACTION());
    QAction* cutAction = menu_->addAction(tr_values::CUT_ACTION());
    QAction* pasteAction = menu_->addAction(tr_values::PASTE_ACTION());
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
