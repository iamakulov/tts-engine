#include <QString>
#include <QtTest>

#include "../../src/collections/List.h"
#include "../../src/structures/token.cpp"
#include "../../src/lettersplitter.cpp"

Q_DECLARE_METATYPE_TEMPLATE_1ARG(List)
Q_DECLARE_METATYPE(Token)

class LetterSplitterTest : public QObject
{
    Q_OBJECT

public:
    LetterSplitterTest();

private Q_SLOTS:
    void splitToLetters_data();
    void splitToLetters();
};

LetterSplitterTest::LetterSplitterTest()
{
}

void LetterSplitterTest::splitToLetters_data()
{
    QTest::addColumn<List<Token>>("words");
    QTest::addColumn<List<Token>>("letters");

    QTest::newRow("Plain data")
        << List<Token> {
           Token(TokenName::WORD, "mary"),
           Token(TokenName::WORD, "had"),
           Token(TokenName::WORD, "a"),
           Token(TokenName::WORD, "sheep")
        }
        << List<Token> {
           Token(TokenName::WORD_SEPARATOR),
           Token(TokenName::LETTER, "m"),
           Token(TokenName::LETTER, "a"),
           Token(TokenName::LETTER, "r"),
           Token(TokenName::LETTER, "y"),
           Token(TokenName::WORD_SEPARATOR),
           Token(TokenName::WORD_SEPARATOR),
           Token(TokenName::LETTER, "h"),
           Token(TokenName::LETTER, "a"),
           Token(TokenName::LETTER, "d"),
           Token(TokenName::WORD_SEPARATOR),
           Token(TokenName::WORD_SEPARATOR),
           Token(TokenName::LETTER, "a"),
           Token(TokenName::WORD_SEPARATOR),
           Token(TokenName::WORD_SEPARATOR),
           Token(TokenName::LETTER, "s"),
           Token(TokenName::LETTER, "h"),
           Token(TokenName::LETTER, "e"),
           Token(TokenName::LETTER, "e"),
           Token(TokenName::LETTER, "p"),
           Token(TokenName::WORD_SEPARATOR)
        };

    QTest::newRow("Empty data")
        << List<Token> { }
        << List<Token> { };
}

void LetterSplitterTest::splitToLetters()
{
    QFETCH(List<Token>, words);
    QFETCH(List<Token>, letters);

    auto a = LetterSplitter().splitToLetters(words);

    QCOMPARE(a, letters);
}

QTEST_APPLESS_MAIN(LetterSplitterTest)

#include "tst_lettersplittertest.moc"
