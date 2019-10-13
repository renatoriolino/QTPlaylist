#ifndef CMUSICSERVICESPOTIFY_H
#define CMUSICSERVICESPOTIFY_H

#include <QObject>
#include "imusicservice.h"

#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>

class cMusicServiceSpotify : public iMusicService
{
    Q_OBJECT
public:
    cMusicServiceSpotify();

    virtual void Connect(void);
    virtual bool IsConnected(void);

    virtual bool Search(QString _str);
public slots:
    void granted(void);
    void status_changed(QAbstractOAuth::Status status);

protected:
    bool m_IsConnected = false;

    QOAuth2AuthorizationCodeFlow spotify;
    QOAuthHttpServerReplyHandler *replyHandler = nullptr;
};

#endif // CMUSICSERVICESPOTIFY_H
