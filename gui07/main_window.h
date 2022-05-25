#pragma once

#include <QMainWindow>
#include <QTranslator>
#include <QString>
#include <QTabWidget>
#include <memory>
#include <vector>

#include "key_event_filter.h"
#include "custom_plain_text_edit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSettings;
class KeyEventFilter;
class FinderDialog;
class QWidget;

struct doc_props
{
    QString file_path { };
    bool is_read_only { false };
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onOpenClicked();
    void onOpenReadClicked();
    void onSaveClicked();
    void onNewClicked();
    void onHelpClicked();
    void onQuitClicked();
    void onPrintClicked();
    void onEnglishSelected(bool checked);
    void onDarkSelected(bool checked);
    void onFinderMenuClicked();
    void tabSelected(int index);
    void onLeftClicked();
    void onCenterClicked();
    void onRightClicked();
    void onFontClicked();

private:
    void loadFile(CustomPlainTextEdit* tab);
    void updateBasedOnReadOnlyState();
    void switchLanguage(QString const& language);
    void switchTheme(QString const& theme);
    void closeTab(int index);
    void initShortcuts();
    void initTabs();
    void connectButtons();
    void connectActions();
    void initEventFilter();

    QTranslator translator_ { };
    QString dir_ { };
    KeyEventFilter::hk_map_t hot_keys_ { };
    std::vector<doc_props> docs_ { };
    int current_tab_ { 0 };

    std::unique_ptr<Ui::MainWindow> ui_;
    std::unique_ptr<QSettings> settings_;
    std::shared_ptr<KeyEventFilter> ev_filter_;
    std::unique_ptr<FinderDialog> finder_dialog_;
    std::unique_ptr<QTabWidget> tabs_;
};
