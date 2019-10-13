#ifndef IMUSICSERVICE_H
#define IMUSICSERVICE_H

#include <QObject>

class iMusicService : public QObject
{
    Q_OBJECT
public:
    explicit iMusicService(QObject *parent = nullptr);

    virtual void Connect(void) = 0;
    virtual bool IsConnected(void) = 0;

    virtual bool Search(QString _str) = 0;

    virtual void Play(const QString &_trackId) = 0;
signals:
    void connected();
    void searchResult(QList<QPair<QString,QString>> r);
    void trackAvailableForPlay(const QString &_preview);

public slots:
protected:

};

#endif // IMUSICSERVICE_H
