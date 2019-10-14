#ifndef IMUSICSERVICE_H
#define IMUSICSERVICE_H

#include <QObject>

class iMusicService : public QObject
{
    Q_OBJECT
public:
    struct S_TRACK_DATA
    {
        QString name;
        QString id;
        QString url;
    };

    explicit iMusicService(QObject *parent = nullptr);

    virtual void Connect(void) = 0;
    virtual bool IsConnected(void) = 0;

    virtual bool Search(QString _str) = 0;

    virtual void Play(const QString &_trackId, int _row) = 0;
signals:
    void connected();
    void searchResult(const QList<iMusicService::S_TRACK_DATA> &r);
    void trackAvailableForPlay(const QString &_preview);
    void trackNotAvailable();

public slots:
protected:

};

#endif // IMUSICSERVICE_H
