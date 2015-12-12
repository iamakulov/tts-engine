#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include "collections/List.cpp"
#include "structures/token.h"
#include "structures/transformationrule.h"


class Transformer
{
public:
    Transformer(List<TransformationRule> rules);

    List<Token> transform(List<Token> tokens) const;

private /*methods*/:
    bool ruleMatches(TransformationRule rule, List<Token> tokens);

private /*members*/:
    const List<TransformationRule> m_rules;
};

#endif // TRANSFORMER_H
