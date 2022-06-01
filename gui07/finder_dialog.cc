#include "finder_dialog.h"
#include "ui_finder_dialog.h"

#include <QPushButton>
#include <QStyle>

#include <memory>

#include "finder_widget.h"
#include "global_consts.h"
#include "little_helpers.h"

FinderDialog::FinderDialog(QWidget* parent)
    : QDialog(parent)
    , ui_(std::make_unique<Ui::FinderDialog>())
    , finder_(std::make_unique<FinderWidget>(this))
{
    ui_->setupUi(this);
    setFixedSize(width(), height());

    connect(ui_->exit_button, &QPushButton::clicked, this, &FinderDialog::onExitButtonClicked);
}

FinderDialog::~FinderDialog()
{
}

void FinderDialog::updateTranslatable()
{
    ui_->exit_button->setText(tr_values::FINDER_CLOSE_BTN());
    this->setWindowTitle(tr_values::FINDER_ACTION());
}

void FinderDialog::switchTheme(QString const& theme)
{
    auto lst = this->findChildren<QWidget*>();
    lst.push_back(this);

    processStyleInList(lst, theme);
}

void FinderDialog::onExitButtonClicked()
{
    close();
}
