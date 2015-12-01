#ifndef TEXT_H
#define TEXT_H

#include "collections/List.h"
#include "phrase.h"

class Text
{
public:
    Text(const QString &text);

    List<Phrase> splitToPhrases() const;

private /*methods*/:
    List<QString> splitTextToSentences(const QString &text) const;

private /*members*/:
    QString m_text;
};

#endif // TEXT_H
