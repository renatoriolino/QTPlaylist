#ifndef IMUSICSERVICE_H
#define IMUSICSERVICE_H

#include <QObject>

class iMusicService : public QObject
{
    Q_OBJECT
public:
    explicit iMusicService(QObject *parent = nullptr);

signals:

public slots:
};

#endif // IMUSICSERVICE_H
