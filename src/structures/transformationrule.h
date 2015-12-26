#ifndef TRANSFORMATIONRULE_H
#define TRANSFORMATIONRULE_H

#include <functional>
#include "../collections/List.cpp"
#include "token.h"


class TransformationRule
{
public:
    typedef std::function<bool(Token)> MatcherType;
    typedef List<std::function<bool(Token)>> MatchingSequenceType;
    typedef std::function<List<Token>(List<Token>)> HandlerType;

    TransformationRule(MatchingSequenceType matchingSequence = MatchingSequenceType(), HandlerType handler = HandlerType());

    MatchingSequenceType matchingSequence() const;
    HandlerType handler() const;

private:
    MatchingSequenceType m_matchingSequence;
    HandlerType m_handler;
};

#endif // TRANSFORMATIONRULE_H
