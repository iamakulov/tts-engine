#include "reassemblerrule.h"

ReassemblerRule::ReassemblerRule(MatchingSequenceType matchingSequence, HandlerType handler)
    : m_matchingSequence(matchingSequence), m_handler(handler)
{

}

ReassemblerRule::MatchingSequenceType ReassemblerRule::matchingSequence() const
{
    return m_matchingSequence;
}

ReassemblerRule::HandlerType ReassemblerRule::handler() const
{
    return m_handler;
}
