#include "audioqueue.h"
#include <QSharedPointer>

AudioQueue::AudioQueue(List<AudioDefinition> audios, QObject *parent)
    : QObject(parent), m_audios(audios)
{
    m_mediaPlaylist.addMedia(audios.mappedTo<QMediaContent>([](AudioDefinition def) {
       return QMediaContent(def.file());
    }));

    m_mediaPlayer.setPlaylist(&m_mediaPlaylist);
    m_mediaPlayer.setNotifyInterval(20);
}

void AudioQueue::play()
{
    m_currentAudio = 0;
    playAudio(m_currentAudio);
}

void AudioQueue::playAudio(int index)
{
    AudioDefinition definition = m_audios[index];
    int startTime = definition.startTime();
    int endTime = definition.endTime();

    m_mediaPlaylist.setCurrentIndex(index);
    m_mediaPlayer.setPosition(startTime);

    QSharedPointer<QMetaObject::Connection> connection(new QMetaObject::Connection);
    *connection = connect(&m_mediaPlayer, &QMediaPlayer::positionChanged, [this, endTime, connection]() {
        if (endTime < m_mediaPlayer.position()) {
            m_mediaPlayer.stop();
            disconnect(*connection);
            switchAudio();
        }
    });

    m_mediaPlayer.play();
}

void AudioQueue::switchAudio()
{
    ++m_currentAudio;
    if (m_currentAudio < m_audios.length()) {
        playAudio(m_currentAudio);
    } else {
        emit queueFinished();
    }
}
