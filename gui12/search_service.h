#pragma once

#include <QObject>

class QString;
class QNetworkAccessManager;
class QNetworkReply;

class SearchService : public QObject
{
    Q_OBJECT

public:
    explicit SearchService(QObject* parent = nullptr);
    ~SearchService();

    Q_INVOKABLE
    void searchPictures(QString const& to_find);

signals:
    void itemFound(QString const& url);
    void searchDone(bool succeed, QString const& error = "");

private slots:
    void replyFinished();

private:
    static inline QString const kSourceAddress { "https://yandex.ru/images/search?text=" };

    QNetworkAccessManager* manager { nullptr };
    QNetworkReply* reply { nullptr };
};
