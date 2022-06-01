#pragma once

#include <QMainWindow>
#include <QTranslator>
#include <QString>
#include <QTabWidget>
#include <QTextCharFormat>

#include <memory>
#include <vector>

#include "key_event_filter.h"
#include "custom_plain_text_edit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSettings;
class FinderDialog;
class SettingsDialog;
class QWidget;

struct DocProps
{
    QString file_path { };
    bool is_read_only { false };
};

struct CharFmt
{
    QTextCharFormat fmt { };
    Qt::Alignment align { };
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void switchLanguage(QString const& lang);
    void switchTheme(QString const& theme);
    QSettings* getSettings();

private slots:
    void onOpenClicked();
    void onOpenReadClicked();
    void onSaveClicked();
    void onNewClicked();
    void onHelpClicked();
    void onQuitClicked();
    void onPrintClicked();
    void onSettingsClicked();
    void onFinderMenuClicked();
    void tabSelected(int index);
    void onLeftClicked();
    void onCenterClicked();
    void onRightClicked();
    void onFontClicked();
    void onCopyFormatClicked();
    void onApplyFormatClicked();

private:
    void loadFile(CustomPlainTextEdit* tab);
    void updateBasedOnReadOnlyState();
    void closeTab(int index);
    void initShortcuts();
    void initTabs();
    void connectButtons();
    void connectActions();
    void initEventFilter();

    QTranslator translator_ { };
    QString dir_ { };
    KeyEventFilter::HotkeysMap hot_keys_ { };
    std::vector<DocProps> docs_ { };
    int current_tab_ = 0;
    CharFmt fmt_ { };

    std::unique_ptr<Ui::MainWindow> ui_;
    std::unique_ptr<QSettings> settings_;
    std::unique_ptr<FinderDialog> finder_dialog_;
    std::unique_ptr<SettingsDialog> settings_dialog_;
    std::shared_ptr<KeyEventFilter> ev_filter_;
    std::unique_ptr<QTabWidget> tabs_;
};
