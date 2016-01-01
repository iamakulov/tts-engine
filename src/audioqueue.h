#ifndef AUDIOQUEUE_H
#define AUDIOQUEUE_H

#include <QObject>

class AudioQueue : public QObject
{
    Q_OBJECT
public:
    explicit AudioQueue(QObject *parent = 0);

signals:

public slots:
};

#endif // AUDIOQUEUE_H