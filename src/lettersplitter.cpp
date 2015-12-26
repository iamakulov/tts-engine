#include "lettersplitter.h"

LetterSplitter::LetterSplitter()
{

}

List<Token> LetterSplitter::splitToLetters(List<Token> words) const
{
    auto collector = List<Token>();
    return words.reducedTo<List<Token>>([](List<Token> collector, Token word) {
        QStringList characterList = word.data().split("", QString::SkipEmptyParts);
        List<Token> letters = List<QString>(characterList).mappedTo<Token>([](QString character) {
            return Token(TokenName::LETTER, character);
        });

        List<Token> wordSeparator{ Token(TokenName::WORD_SEPARATOR) };

        return collector
            + wordSeparator
            + letters
            + wordSeparator;
    }, collector);
}

