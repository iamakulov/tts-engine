#include "audioqueue.h"
#include <QSharedPointer>

AudioQueue::AudioQueue(List<AudioDefinition> audios, QObject *parent)
    : QObject(parent), m_audios(audios)
{
    m_mediaPlayer.setNotifyInterval(20);
}

void AudioQueue::play()
{
    m_currentAudio = 0;
    playAudio(m_audios[m_currentAudio]);
}

void AudioQueue::playAudio(AudioDefinition audio)
{
    m_mediaPlayer.setMedia(QUrl::fromLocalFile(audio.file()));
    m_mediaPlayer.setPosition(audio.startTime());

    QSharedPointer<QMetaObject::Connection> connection(new QMetaObject::Connection);
    *connection = connect(&m_mediaPlayer, &QMediaPlayer::positionChanged, [this, audio, connection]() {
        if (audio.endTime() < m_mediaPlayer.position()) {
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
        playAudio(m_audios[m_currentAudio]);
    } else {
        emit queueFinished();
    }
}
