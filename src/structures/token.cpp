#include "token.h"

Token::Token(QString name, QString data)
    : m_name(name), m_data(data)
{

}

QString Token::name() const
{
    return m_name;
}

QString Token::data() const
{
    return m_data;
}

bool Token::operator==(const Token &other) const
{
    return m_name == other.m_name && m_data == other.m_data;
}
