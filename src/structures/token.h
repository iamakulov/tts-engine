#ifndef TOKEN_H
#define TOKEN_H

#include <QString>


class Token
{
public:
    Token(QString name = QString(), QString data = QString());

    QString name() const;
    QString data() const;

    bool operator==(const Token &other) const;

private:
    QString m_name;
    QString m_data;
};

#endif // TOKEN_H
