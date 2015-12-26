#ifndef LETTERSPLITTER_H
#define LETTERSPLITTER_H

#include "collections/List.h"
#include "structures/token.h"

class LetterSplitter
{
public:
    LetterSplitter();

    // The method is kept as a non-static member to keep consistency with other processing classes (Tokenizer, Transformer etc)
    List<Token> splitToLetters(List<Token> words) const;
};

#endif // LETTERSPLITTER_H
