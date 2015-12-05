#ifndef PHRASE_H
#define PHRASE_H

#include <QString>

class Phrase
{
public:
    enum class Kind {
        Declarative,
        Exclamatory,
        Interrogative
    };

    Phrase(const QString& text, const Kind &kind);
    static Phrase parse(const QString &rawPhrase);

private /*members*/:
    QString m_text;
    Kind m_kind;
};

#endif // PHRASE_H
