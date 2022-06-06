#include "custom_text_edit.h"

#include <QApplication>
#include <QContextMenuEvent>
#include <QTextEdit>
#include <QClipboard>
#include <QAction>
#include <QWidget>
#include <QMenu>

#include "global_consts.h"

CustomTextEdit::CustomTextEdit(QWidget* parent) : QTextEdit(parent)
{
    menu = new QMenu(this);
    setViewportMargins(0, 0, 15, 0);
    auto const copyAction = menu->addAction(tr_values::COPY_ACTION());
    auto const cutAction = menu->addAction(tr_values::CUT_ACTION());
    auto const pasteAction = menu->addAction(tr_values::PASTE_ACTION());
    connect(copyAction, &QAction::triggered, this, &CustomTextEdit::copyText);
    connect(cutAction, &QAction::triggered, this, &CustomTextEdit::cutText);
    connect(pasteAction, &QAction::triggered, this, &CustomTextEdit::pasteText);
}

void CustomTextEdit::contextMenuEvent(QContextMenuEvent* event)
{
    menu->exec(event->globalPos());
}

void CustomTextEdit::copyText()
{
    auto const str = this->textCursor().selectedText();
    qApp->clipboard()->setText(str);
}

void CustomTextEdit::cutText()
{
    auto const str = this->textCursor().selectedText();
    this->textCursor().removeSelectedText();
    qApp->clipboard()->setText(str);
}

void CustomTextEdit::pasteText()
{
    auto const str = qApp->clipboard()->text();
    this->textCursor().insertText(str);
}
