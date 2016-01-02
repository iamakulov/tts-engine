#ifndef LTSRULELOADER_H
#define LTSRULELOADER_H

#include <QSharedPointer>
#include <QTextStream>
#include <QRegularExpression>
#include "../collections/List.h"
#include "../structures/ltsrule.h"
#include "../structures/token.h"

class LtsRuleLoader
{
public:
    LtsRuleLoader(QSharedPointer<QTextStream> inputStream);

    List<LtsRule> getData();

private /*methods*/:
    bool reportDataErrors(const QJsonDocument &json) const;

    bool validateLtsRule(const QString &string) const;
    LtsRule parseLtsRule(const QString &string) const;

    List<Token> convertLettersToTokens(const QStringList &letters) const;
    List<Token> convertSoundsToTokens(const QStringList &sounds) const;

private /*members*/:
    QSharedPointer<QTextStream> m_inputStream;

    const QRegularExpression LTS_RULE_PATTERN = QRegularExpression(
        "^([\\w#]*)"         // amazi
        "\\[(\\w+)\\]"       // [ngn]
        "([\\w#]*)"          // ess
        "\\s?=\\s?"          // =
        "(\\S+(\\s\\S+)*)$"  // n n
    , QRegularExpression::UseUnicodePropertiesOption);
};

#endif // LTSRULELOADER_H
