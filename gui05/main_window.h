#pragma once

#include <QMainWindow>
#include <QTranslator>
#include <QString>
#include <QList>
#include <memory>
#include <QKeyEvent>

#include "key_event_filter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSettings;
class QEvent;
class KeyEventFilter;
class FinderDialog;
class QWidget;

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
    void onEnglishSelected(bool checked);
    void onDarkSelected(bool checked);
    void onFinderMenuClicked();

private:
    void loadFile();
    void updateBasedOnReadOnlyState();
    void switchLanguage(QString const& language);
    void switchTheme(QString const& theme);

    QTranslator translator_ { };
    QString file_path_ { };
    QString dir_ { };
    KeyEventFilter::hk_map_t hot_keys_ { };

    bool is_read_only_ { false };

    std::unique_ptr<Ui::MainWindow> ui_;
    std::unique_ptr<QSettings> settings_;
    std::shared_ptr<KeyEventFilter> ev_filter_;
    std::unique_ptr<FinderDialog> finder_dialog_;
};
