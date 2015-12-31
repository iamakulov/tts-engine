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


inline uint qHash(const List<Token> &list, uint seed) {
    QString sounds = list.reducedTo<QString>([](QString collector, Token token) {
        return collector + " " + token.name() + " (" + token.data() + ")";
    }, QString());

    return qHash(sounds, seed);
}

#endif // AUDIOCONVERTOR_H
