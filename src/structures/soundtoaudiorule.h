#ifndef SOUNDTOAUDIORULE_H
#define SOUNDTOAUDIORULE_H

#include "../collections/List.h"
#include "token.h"
#include "audiodefinition.h"

class SoundToAudioRule
{
public:
    SoundToAudioRule(List<Token> sounds = List<Token>(), AudioDefinition audio = AudioDefinition());

    List<Token> sounds() const;
    AudioDefinition audio() const;

    bool operator==(const SoundToAudioRule &other) const;

private:
    List<Token> m_sounds;
    AudioDefinition m_audio;
};

#endif // SOUNDTOAUDIORULE_H
