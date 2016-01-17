#include "transformer.h"

Transformer::Transformer(List<TransformationRule> rules)
    : m_rules(rules)
{

}

List<Token> Transformer::transform(List<Token> tokens) const
{
    List<Token> transformedTokens = tokens;
    for (int i = 0; i < transformedTokens.length(); ++i) {
        m_rules
            // Process each rule
            .forEach([&transformedTokens, i, this](TransformationRule rule) {
                bool ruleNotLongerTokens = rule.matchingSequence().length() <= transformedTokens.length() - i;
                auto tokensToMatch = transformedTokens.mid(i, rule.matchingSequence().length());
                if (ruleNotLongerTokens && ruleMatches(rule, tokensToMatch)) {
                    auto newTokens = rule.handler()(tokensToMatch);
                    transformedTokens = transformedTokens.mid(0, i) + newTokens + transformedTokens.mid(i + rule.matchingSequence().length());
                }
            });
    }

    return transformedTokens;
}

bool Transformer::ruleMatches(TransformationRule rule, List<Token> tokens)
{
    return rule.matchingSequence().filtered([tokens](auto matchFunction, int i) {
        return matchFunction(tokens[i]);
    }).length() == rule.matchingSequence().length();
}

