#pragma once

#include <QMainWindow>
#include <QTranslator>
#include <QString>
#include <memory>
#include <QKeyEvent>

#include "key_event_filter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSettings;
class QEvent;
class KeyEventFilter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_open_clicked();
    void on_open_read_clicked();
    void on_save_clicked();
    void on_new_clicked();
    void on_help_clicked();
    void on_quit_clicked();
    void on_english_selected(bool checked);

private:
    void load_file();
    void update_based_on_read_only_state();
    void switch_language(QString language);

    QTranslator translator_ { };
    QString file_path_ { };
    QString dir_ { };
    KeyEventFilter::hk_map_t hot_keys_ { };

    bool is_read_only_;

    std::unique_ptr<Ui::MainWindow> ui_ { nullptr };
    std::unique_ptr<QSettings> settings_ { nullptr };
    std::shared_ptr<KeyEventFilter> ev_filter_ { nullptr };

};
