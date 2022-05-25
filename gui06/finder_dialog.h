#pragma once

#include <memory>

#include <QDialog>
#include <QString>

namespace Ui { class FinderDialog; }

class FinderWidget;

class FinderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FinderDialog(QWidget* parent = nullptr);
    ~FinderDialog();
    void updateTranslatable();
    void switchTheme(QString const& theme);

private slots:
    void onExitButtonClicked();

private:
    std::unique_ptr<Ui::FinderDialog> ui_;
    std::unique_ptr<FinderWidget> finder_;
};
