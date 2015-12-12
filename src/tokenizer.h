#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QPointer>
#include <QRegularExpression>
#include "collections/List.h"
#include "structures/token.h"
#include "structures/tokendefinition.h"

class Tokenizer
{
public:
    Tokenizer(const List<TokenDefinition> &tokenDefinitions);

    List<Token> tokenize(const QString &text) const;

private:
    const List<TokenDefinition> m_tokenDefinitions;
};

#endif // TOKENIZER_H
