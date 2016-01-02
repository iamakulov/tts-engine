#include "player.h"
#include "audioqueue.h"

Player::Player()
{

}

void Player::play(List<AudioDefinition> audios) const
{
    AudioQueue *queue = new AudioQueue(audios);
    queue->play();
    QObject::connect(queue, &AudioQueue::queueFinished, queue, &AudioQueue::deleteLater);
}
