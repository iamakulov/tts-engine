#ifndef REASSEMBLERRULE_H
#define REASSEMBLERRULE_H

#include <functional>
#include "../collections/List.cpp"
#include "token.h"


class ReassemblerRule
{
public:
    typedef List<std::function<bool(Token)>> MatchingSequenceType;
    typedef std::function<List<Token>(List<Token>)> HandlerType;

    ReassemblerRule(MatchingSequenceType matchingSequence = MatchingSequenceType(), HandlerType handler = HandlerType());

    MatchingSequenceType matchingSequence() const;
    HandlerType handler() const;

private:
    MatchingSequenceType m_matchingSequence;
    HandlerType m_handler;
};

#endif // REASSEMBLERRULE_H
