#include "phrase.h"
#include <QPair>
#include "collections/List.h"

Phrase::Phrase(const QString &text, const Kind &kind)
    : m_text(text), m_kind(kind)
{

}

Phrase Phrase::parse(const QString &rawPhrase)
{
    const List<QPair<QString, Phrase::Kind>> types = {
        qMakePair(QString("..."), Phrase::Kind::Declarative),
        qMakePair(QString("?!"), Phrase::Kind::Interrogative),
        qMakePair(QString("?"), Phrase::Kind::Interrogative),
        qMakePair(QString("!"), Phrase::Kind::Exclamatory),
        qMakePair(QString("."), Phrase::Kind::Declarative)
    };

    auto match = types.filter([rawPhrase](auto pair) {
        return rawPhrase.endsWith(pair.first);
    })[0];
    QString ending = match.first;
    Phrase::Kind kind = match.second;

    QString text = rawPhrase;
    text.remove(QRegExp(ending + "$"));

    return Phrase(text, kind);
}

