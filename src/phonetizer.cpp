#include "phonetizer.h"
#include "transformer.h"
#include "structures/transformationrule.h"

Phonetizer::Phonetizer(List<LtsRule> rules)
    : m_rules(rules)
{

}

List<Token> Phonetizer::phonetize(List<Token> letters) const
{
    List<TransformationRule> transformationRules = m_rules.mappedTo<TransformationRule>([](LtsRule ltsRule) {
        auto letterSequence = List<Token>(ltsRule.previousContext() + ltsRule.target() + ltsRule.nextContext());
        auto matchingSequence = letterSequence
            .mappedTo<TransformationRule::MatcherType>([letterSequence](Token letter) {
                return [letter](Token tokenToMatch) {
                    return tokenToMatch == letter;
                };
            });

        auto prevContextLength = ltsRule.previousContext().length();
        auto targetLength = ltsRule.target().length();
        auto sounds = ltsRule.sounds();
        auto handler = [prevContextLength, targetLength, sounds](List<Token> tokens) {
            return tokens.mid(0, prevContextLength) + sounds + tokens.mid(prevContextLength + targetLength);
        };

        return TransformationRule(matchingSequence, handler);
    });

    // The rule to remove all unrecognized letters
    TransformationRule letterRemovalRule = TransformationRule(
        TransformationRule::MatchingSequenceType { [](Token token) { return token.name() == TokenName::LETTER; } },
        [](List<Token>) { return List<Token>(); }
    );

    auto letterSounds = Transformer(transformationRules).transform(letters);
    auto sounds = Transformer(List<TransformationRule> { letterRemovalRule }).transform(letterSounds);

    return sounds;
}
