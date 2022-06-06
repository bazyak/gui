#pragma once

#include <QMainWindow>

#include <memory>

namespace Ui { class SettingsDialog; }

class QString;
class QLineEdit;
class KeyEventFilter;
class QKeySequence;

class SettingsDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);
    ~SettingsDialog();

signals:
    void englishSelected(bool checked);
    void darkSelected(bool checked);
    void changeHotKey(QString const& objName, QKeySequence const& keySeq);

private slots:
    void onCloseClicked();
    void onTranslate();
    void onChangeTheme(QString const& theme);
    void onUpdateLanguageSelector(QString const& language);
    void onUpdateThemeSelector(QString const& theme);
    void onUpdateLineEditPlaceholderText(QString const& name, QString const& text);

private:
    std::unique_ptr<Ui::SettingsDialog> ui;
    KeyEventFilter* evFilter { nullptr };
};
