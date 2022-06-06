#pragma once

#include <QMainWindow>
#include <QTranslator>
#include <QString>
#include <QTabWidget>
#include <QTextCharFormat>

#include <memory>
#include <unordered_map>
#include <vector>

#include "custom_text_edit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSettings;
class SettingsDialog;
class QWidget;
class QShortcut;
class QKeySequence;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void translate();
    void changeTheme(QString const& theme);
    void updateLanguageSelector(QString const& language);
    void updateThemeSelector(QString const& theme);
    void updateLineEditPlaceholderText(QString const& name, QString const& text);

private slots:
    void onOpenClicked();
    void onOpenReadClicked();
    void onSaveClicked();
    void onSaveAsClicked();
    void onNewClicked();
    void onHelpClicked();
    void onQuitClicked();
    void onPrintClicked();
    void onSettingsClicked();
    void tabSelected(int index);
    void onLeftClicked();
    void onCenterClicked();
    void onRightClicked();
    void onFontClicked();
    void onCopyFormatClicked();
    void onApplyFormatClicked();
    void onDateClicked();
    void onTimeClicked();
    void onTextChanged();

    // for settings dialog
    void onEnglishSelected(bool checked);
    void onDarkSelected(bool checked);
    void onChangeHotKey(QString const& objName, QKeySequence const& keySeq);

private:
    using HotkeysMap = std::unordered_map<QString, QShortcut*>;

    struct DocProps
    {
        QString filePath { };
        bool isReadOnly { false };
        bool isSaved { true };
    };
    struct CharFmt
    {
        QTextCharFormat fmt { };
        Qt::Alignment align { };
    };

    void switchLanguage(QString const& lang);
    void switchTheme(QString const& theme);
    bool loadFile(CustomTextEdit* tab);
    void saveFile(CustomTextEdit* tab, bool saveAs = true);
    void updateBasedOnReadOnlyState(bool onlyCurrent = true);
    void closeTab(int index, bool withoutSave = false);
    void initShortcuts();
    void initTabs();
    void connectButtons();
    void connectActions();
    void prepareSettingsDialog();

    QString dir { };
    QString lang { };
    QString theme { };

    QTranslator translator { };
    HotkeysMap hotKeys { };
    std::vector<DocProps> docs { };
    CharFmt fmt { };
    int curTabIdx = -1;

    std::unique_ptr<Ui::MainWindow> ui;
    QSettings* settings { nullptr };
    SettingsDialog* settingsDialog { nullptr };
    QTabWidget* tabs { nullptr };
};
