#include "cmusicservicespotify.h"

#include <QSettings>
#include <QDesktopServices>
#include <QNetworkReply>
#include <QOAuthHttpServerReplyHandler>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

cMusicServiceSpotify::cMusicServiceSpotify()
{
}

void cMusicServiceSpotify::Connect()
{
    if (IsConnected())
        return;

    QSettings settings;

    if (replyHandler == nullptr)
    {
        replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
        spotify.setReplyHandler(replyHandler);
    }
    spotify.setAuthorizationUrl(QUrl("https://accounts.spotify.com/authorize"));
    spotify.setAccessTokenUrl(QUrl("https://accounts.spotify.com/api/token"));

    spotify.setClientIdentifier(settings.value("Spotify/id", "").toString());
    spotify.setClientIdentifierSharedKey(settings.value("Spotify/secret", "").toString());

    spotify.setScope("user-read-private user-top-read playlist-read-private playlist-modify-public playlist-modify-private");

    connect(&spotify, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);

    connect(&spotify, &QOAuth2AuthorizationCodeFlow::statusChanged,
            this, &cMusicServiceSpotify::status_changed);

    connect(&spotify, &QOAuth2AuthorizationCodeFlow::granted, this, &cMusicServiceSpotify::granted);

    spotify.grant();
}

bool cMusicServiceSpotify::IsConnected()
{
    return m_IsConnected;
}

bool cMusicServiceSpotify::Search(QString _str)
{
    Connect();
    if (!m_IsConnected)
        return false;

    _str.replace(" ", "+");
    QUrl u(QString("https://api.spotify.com/v1/search?q=")+_str+QString("&type=track"));

    auto reply = spotify.get(u);

    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() != QNetworkReply::NoError) {
           return;
        }

        const auto data = reply->readAll();

        const auto document = QJsonDocument::fromJson(data);
        const auto root = document.object();

        QList<QString> sr;
        const auto items = root["tracks"]["items"].toArray();
        for (auto o : items)
            sr.push_back(o["name"].toString());

        searchResult(sr);

        reply->deleteLater();
    });

    return true;
}

void cMusicServiceSpotify::granted()
{
    qDebug() << "Connected!";

    m_IsConnected = true;
}

void cMusicServiceSpotify::status_changed(QAbstractOAuth::Status status)
{
    if (status == QAbstractOAuth::Status::Granted)
        qDebug() << "Status changed: granted";

    else if (status == QAbstractOAuth::Status::TemporaryCredentialsReceived)
        qDebug() << "Status changed: tempCred";
    else if (status == QAbstractOAuth::Status::NotAuthenticated)
        qDebug() << "Status changed: not auth";
    else
        qDebug() << "Status changed: else";
}
