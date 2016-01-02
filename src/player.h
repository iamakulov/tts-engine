#ifndef PLAYER_H
#define PLAYER_H

#include <QMediaPlayer>
#include "collections/List.h"
#include "structures/audiodefinition.h"

class Player
{
public:
    Player();

    void play(List<AudioDefinition> audios) const;
};

#endif // PLAYER_H
