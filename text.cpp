#include "text.h"
#include <QRegularExpression>

Text::Text(const QString &text)
    : m_text(text)
{

}

List<Phrase> Text::splitToPhrases() const
{
    return splitTextToSentences(m_text)
        .mapTo<Phrase>([](QString phrase) {
            return Phrase(phrase);
        });
}

List<QString> Text::splitTextToSentences(const QString &text) const
{
    const QString pattern = "[\\.\\!\\?]+\\s";
    List<QString> sentences = List<QString>(text.split(QRegExp(pattern)));
    QStringList separators = QRegularExpression(pattern).match(text).capturedTexts();

    return sentences.map([separators](QString str, int index) {
        return (str + separators[index]).trimmed(); // .trimmed() is used to remove extra spaces present in the separator
    });
}

