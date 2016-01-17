#include "transformer.h"

Transformer::Transformer(List<TransformationRule> rules)
    : m_rules(rules)
{

}

List<Token> Transformer::transform(List<Token> tokens) const
{
    List<Token> transformedTokens = tokens;
    List<Token> previouslyTransformedTokens;

    // When some tokens are replaced with a smaller number of tokens,
    // some new tokens may appear before the position where transformations are being performed.
    // Therefore, such tokens will not be transformed.
    // To ensure that all tokens are always transformed, the transformation is repeated until the list stops changing.
    do {
        previouslyTransformedTokens = transformedTokens;

        for (int i = 0; i < transformedTokens.length(); ++i) {
            m_rules
                .forEach([&transformedTokens, i, this](TransformationRule rule) {
                    bool ruleNotLongerTokens = rule.matchingSequence().length() <= transformedTokens.length() - i;
                    auto tokensToMatch = transformedTokens.mid(i, rule.matchingSequence().length());

                    if (ruleNotLongerTokens && ruleMatches(rule, tokensToMatch)) {
                        auto newTokens = rule.handler()(tokensToMatch);
                        transformedTokens = transformedTokens.mid(0, i) + newTokens + transformedTokens.mid(i + rule.matchingSequence().length());
                    }
                });
        }
    } while (previouslyTransformedTokens != transformedTokens);

    return transformedTokens;
}

bool Transformer::ruleMatches(TransformationRule rule, List<Token> tokens)
{
    return rule.matchingSequence().filtered([tokens](auto matchFunction, int i) {
        return matchFunction(tokens[i]);
    }).length() == rule.matchingSequence().length();
}

