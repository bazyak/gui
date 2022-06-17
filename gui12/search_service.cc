#include "search_service.h"

#include <QString>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QVariant>
#include <QTextDocument>
#include <QTextDocumentFragment>

#include <vector>

#include "html_parser.h"

SearchService::SearchService(QObject* parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &SearchService::replyFinished);
}

SearchService::~SearchService()
{
}

void SearchService::searchPictures(QString const& to_find)
{
    QUrl const url(kSourceAddress + to_find);
    if (reply) delete reply;
    reply = manager->get(QNetworkRequest(url));
}

void SearchService::replyFinished()
{
    auto const statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if (statusCode == 302)
    {
        auto const newUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        QNetworkRequest newRequest(newUrl);
        manager->get(newRequest);
        return;
    }
    if (reply->error() == QNetworkReply::NoError)
    {
        emit searchDone(true);
        QByteArray const bytes = reply->readAll();
        auto const str = QString::fromUtf8(bytes.data(), bytes.size());
        for (auto const& item : HtmlParser::Parse(str))
        {
            auto const good = QTextDocumentFragment::fromHtml(item).toPlainText();
            emit itemFound(good);
        }
    }
    else
    {
        emit searchDone(false, reply->errorString());
    }
}
