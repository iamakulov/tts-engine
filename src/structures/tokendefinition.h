#ifndef TOKENDEFINITION_H
#define TOKENDEFINITION_H

#include <QRegularExpression>
#include <QString>


class TokenDefinition
{
public:
    TokenDefinition(QString name = QString(), QString pattern = QString());

    QString name() const;
    QString pattern() const;

    bool matches(const QString &text);

    bool operator==(const TokenDefinition &other) const;

private:
    QString m_name;
    QString m_pattern;
    QRegularExpression m_regExp;
};

#endif // TOKENDEFINITION_H
