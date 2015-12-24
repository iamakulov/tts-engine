#include "transformationrule.h"

TransformationRule::TransformationRule(MatchingSequenceType matchingSequence, HandlerType handler)
    : m_matchingSequence(matchingSequence), m_handler(handler)
{

}

TransformationRule::MatchingSequenceType TransformationRule::matchingSequence() const
{
    return m_matchingSequence;
}

TransformationRule::HandlerType TransformationRule::handler() const
{
    return m_handler;
}
