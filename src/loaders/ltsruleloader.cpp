#include "ltsruleloader.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include "../structures/token.h"

LtsRuleLoader::LtsRuleLoader(QSharedPointer<QTextStream> inputStream)
    : m_inputStream(inputStream)
{

}

List<LtsRule> LtsRuleLoader::getData()
{
    QString textData = m_inputStream->readAll();
    QJsonDocument json = QJsonDocument::fromJson(textData.toUtf8());

    bool isDataValid = reportDataErrors(json);
    if (!isDataValid) {
        return List<LtsRule>();
    }

    return List<QVariant>(json.array().toVariantList())
        .mappedTo<LtsRule>([this](const QVariant &arrayElement) {
            QString value = arrayElement.toString().simplified();
            return parseLtsRule(value);
        });
}

bool LtsRuleLoader::reportDataErrors(const QJsonDocument &json) const
{
    if (!json.isArray()) {
        qWarning() << "LtsRuleLoader#reportDataErrors: the passed text stream cannot be converted to a JSON array.";
        qWarning() << "The parsed JSON content:" << json.toJson();
        return false;
    }

    List<QVariant> allItems = json.array().toVariantList();
    List<QVariant> validItems = allItems
        .filtered([this](const QVariant &arrayElement) {
            QString value = arrayElement.toString().simplified();

            if (!validateLtsRule(value)) {
                qWarning() << "LtsRuleLoader#reportDataErrors: an array value cannot be parsed to a letter-to-sound rule.";
                qWarning() << "The rule must look like 'amazi[ngn]ess = n n'.";
                qWarning() << "The parsed rule:" << value;
                return false;
            }

            return true;
        });

    if (validItems.length() != allItems.length()) {
        return false;
    }

    return true;
}

bool LtsRuleLoader::validateLtsRule(const QString &string) const
{
    return LTS_RULE_PATTERN.match(string).hasMatch();
}

LtsRule LtsRuleLoader::parseLtsRule(const QString &string) const
{
    QRegularExpressionMatch match = LTS_RULE_PATTERN.match(string);

    QStringList prevLetters = match.captured(1).split("", QString::SkipEmptyParts);
    auto prevTokens = convertLettersToTokens(prevLetters);

    QStringList targetLetters = match.captured(2).split("", QString::SkipEmptyParts);
    auto targetTokens = convertLettersToTokens(targetLetters);

    QStringList nextLetters = match.captured(3).split("", QString::SkipEmptyParts);
    auto nextTokens = convertLettersToTokens(nextLetters);

    QStringList sounds = match.captured(4).split(" ", QString::SkipEmptyParts);
    auto soundTokens = convertSoundsToTokens(sounds);

    return LtsRule(prevTokens, targetTokens, nextTokens, soundTokens);
}

List<Token> LtsRuleLoader::convertLettersToTokens(const QStringList &letters) const
{
    return List<QString>(letters).mappedTo<Token>([](const QString &letter) {
        if (letter == "#") {
            return Token(TokenName::WORD_SEPARATOR);
        } else {
            return Token(TokenName::LETTER, letter);
        }
    });
}

List<Token> LtsRuleLoader::convertSoundsToTokens(const QStringList &sounds) const
{
    return List<QString>(sounds).mappedTo<Token>([](const QString &sound) {
        return Token(TokenName::SOUND, sound);
    });
}

