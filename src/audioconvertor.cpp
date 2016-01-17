#include "audioconvertor.h"
#include "transformer.h"
#include "structures/transformationrule.h"

AudioConvertor::AudioConvertor(const List<SoundToAudioRule> &rules)
{
    // Sort rules by the sound count in descending order
    m_rules = rules.sorted([](SoundToAudioRule left, SoundToAudioRule right) {
        return right.sounds().length() < left.sounds().length();
    });
}

List<AudioDefinition> AudioConvertor::convert(List<Token> sounds) const
{
    // Despite the call AudioConvertor return a list of audios and not tokens,
    // the Transformer usage is the most efficient way to convert sounds to audios
    List<TransformationRule> transformationRules = m_rules.mappedTo<TransformationRule>([](SoundToAudioRule soundToAudioRule, int i) {
        // Match the tokens stored in the soundToAudioRule
        auto matchingSequence = soundToAudioRule.sounds().mappedTo<TransformationRule::MatcherType>([](Token sound) {
            return [sound](Token tokenToMatch) {
                return tokenToMatch == sound;
            };
        });

        // Replace each match with the index of the sound-to-audio rule
        auto handler = [i](List<Token> tokens) {
            Q_UNUSED(tokens);
            // Wow · Such hacks · Much patterns
            return List<Token> { Token("AUDIO_ITEM", QString::number(i)) };
        };

        return TransformationRule(matchingSequence, handler);
    });

    // The rule to remove all unrecognized letters
    TransformationRule soundRemovalRule = TransformationRule(
        TransformationRule::MatchingSequenceType { [](Token token) { return token.name() == TokenName::SOUND; } },
        [](List<Token>) { return List<Token>(); }
    );

    auto soundAudios = Transformer(transformationRules).transform(sounds);
    auto audios = Transformer(List<TransformationRule> { soundRemovalRule }).transform(soundAudios);

    return audios.mappedTo<AudioDefinition>([this](Token audio) {
        return m_rules[audio.data().toInt()].audio();
    });
}
