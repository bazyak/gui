#pragma once

#include <QMainWindow>

#include <memory>

namespace Ui { class SettingsDialog; }

class MainWindow;
class QString;
class QLineEdit;

class SettingsDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog();
    void updateTranslatable();
    void switchTheme(QString const& theme);
    bool getLanguage() const;
    QString getTheme() const;
    void setLanguageRadioButton(QString const& language);
    void setThemeRadioButton(QString const& theme);
    QLineEdit* getOpenEdit();
    QLineEdit* getSaveEdit();
    QLineEdit* getNewEdit();
    QLineEdit* getQuitEdit();

private slots:
    void onCloseClicked();
    void onEnglishSelected(bool checked);
    void onDarkSelected(bool checked);

private:
    std::unique_ptr<Ui::SettingsDialog> ui_;
    MainWindow* parent_ { nullptr };
};
