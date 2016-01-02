#include "tokenizer.h"
#include <QPair>
#include <QtDebug>

Tokenizer::Tokenizer(const List<TokenDefinition> &tokenDefinitions)
    : m_tokenDefinitions(tokenDefinitions)
{

}

List<Token> Tokenizer::tokenize(const QString &text) const
{
    QString processedText = text;
    List<Token> result;

    while (processedText.length() > 0) {
        auto matchingDefinitions = m_tokenDefinitions
            .mappedTo<QPair<TokenDefinition, int>>([processedText](auto tokenDefinition) {
                int parsedTokenLength = QRegularExpression("^" + tokenDefinition.pattern(), QRegularExpression::UseUnicodePropertiesOption)
                        .match(processedText).capturedLength();
                return qMakePair(tokenDefinition, parsedTokenLength);
            })
            .sorted([](auto left, auto right) {
                // In descending order
                return left.second > right.second;
            });

        TokenDefinition longestMatchingDefinition = matchingDefinitions[0].first;
        int matchLength = matchingDefinitions[0].second;

        if (matchLength == 0) {
            qWarning() << "Tokenizer::tokenize: no matching tokens found for '" + processedText + "'";
            break;
        }

        QString longestMatchingText = processedText.left(matchLength);
        result.append(Token(longestMatchingDefinition.name(), longestMatchingText));

        processedText = processedText.mid(matchLength);
    }

    return result;
}

