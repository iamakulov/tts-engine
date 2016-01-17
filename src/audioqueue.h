#ifndef AUDIOQUEUE_H
#define AUDIOQUEUE_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "collections/List.h"
#include "structures/audiodefinition.h"

class AudioQueue : public QObject
{
    Q_OBJECT
public:
    explicit AudioQueue(List<AudioDefinition> audios, QObject *parent = 0);

    void play();

signals:
    void queueFinished();

private /*methods*/:
    void playAudio(int index);

private slots:
    void switchAudio();

private /*members*/:
    QMediaPlayer m_mediaPlayer;
    QMediaPlaylist m_mediaPlaylist;
    List<AudioDefinition> m_audios;
    int m_currentAudio = 0;
};

#endif // AUDIOQUEUE_H
