#include "ltsrule.h"

LtsRule::LtsRule(const List<Token> &previousContext, const List<Token> &target, const List<Token> &nextContext, const List<Token> &sounds)
    : m_previousContext(previousContext), m_target(target), m_nextContext(nextContext), m_sounds(sounds)
{

}

List<Token> LtsRule::previousContext() const
{
    return m_previousContext;
}

List<Token> LtsRule::target() const
{
    return m_target;
}

List<Token> LtsRule::nextContext() const
{
    return m_nextContext;
}

List<Token> LtsRule::sounds() const
{
    return m_sounds;
}

bool LtsRule::operator==(const LtsRule &other) const
{
    return m_previousContext == other.m_previousContext
        && m_target == other.m_target
        && m_nextContext == other.m_nextContext
        && m_sounds == other.m_sounds;
}

