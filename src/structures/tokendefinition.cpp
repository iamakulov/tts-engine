#include "tokendefinition.h"

TokenDefinition::TokenDefinition(QString name, QString pattern)
    : m_name(name), m_pattern(pattern), m_regExp("^" + pattern + "$")
{

}

QString TokenDefinition::name() const
{
    return m_name;
}

QString TokenDefinition::pattern() const
{
    return m_pattern;
}

bool TokenDefinition::matches(const QString &text)
{
    return m_regExp.match(text).hasMatch();
}

bool TokenDefinition::operator==(const TokenDefinition &other) const
{
    return m_name == other.m_name && m_pattern == other.m_pattern;
}
