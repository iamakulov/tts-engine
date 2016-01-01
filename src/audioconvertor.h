#ifndef AUDIOCONVERTOR_H
#define AUDIOCONVERTOR_H

#include "collections/List.h"
#include "structures/soundtoaudiorule.h"

class AudioConvertor
{
public:
    AudioConvertor(const List<SoundToAudioRule> &rules);

    List<AudioDefinition> convert(List<Token> sounds) const;

private /*members*/:
    List<SoundToAudioRule> m_rules;
};

#endif // AUDIOCONVERTOR_H
