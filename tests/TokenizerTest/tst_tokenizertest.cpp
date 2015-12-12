#include <QString>
#include <QtTest>

#include "../../src/collections/List.h"
#include "../../src/structures/tokendefinition.cpp"
#include "../../src/structures/token.cpp"
#include "../../src/tokenizer.cpp"

Q_DECLARE_METATYPE_TEMPLATE_1ARG(List)
Q_DECLARE_METATYPE(TokenDefinition)
Q_DECLARE_METATYPE(Token)


class TokenizerTest : public QObject
{
    Q_OBJECT

public:
    TokenizerTest();

private Q_SLOTS:
    void tokenize_data();
    void tokenize();
};

TokenizerTest::TokenizerTest()
{
}

void TokenizerTest::tokenize_data()
{
    QTest::addColumn<List<TokenDefinition>>("tokenDefinitions");
    QTest::addColumn<QString>("text");
    QTest::addColumn<List<Token>>("parsedTokens");

    QTest::newRow("Single existing token")
        << List<TokenDefinition> {
            TokenDefinition("word", "\\w+")
        }
        << "hello"
        << List<Token> {
            Token("word", "hello")
        };

    QTest::newRow("Single absent token")
        << List<TokenDefinition> {
            TokenDefinition("word", "\\w+")
        }
        << " "
        << List<Token> { };

    QTest::newRow("Multiple existing tokens")
        << List<TokenDefinition> {
            TokenDefinition("word", "\\w+"),
            TokenDefinition("space", "\\s+")
        }
        << "hello world"
        << List<Token> {
            Token("word", "hello"),
            Token("space", " "),
            Token("word", "world")
        };

    QTest::newRow("Existing and mixed token mix")
        << List<TokenDefinition> {
            TokenDefinition("word", "\\w+"),
            TokenDefinition("space", "\\s+")
        }
        << "hello, world!"
        << List<Token> {
            Token("word", "hello")
        };
}

void TokenizerTest::tokenize()
{
    QFETCH(List<TokenDefinition>, tokenDefinitions);
    QFETCH(QString, text);
    QFETCH(List<Token>, parsedTokens);
    QCOMPARE(Tokenizer(tokenDefinitions).tokenize(text), parsedTokens);
}

QTEST_APPLESS_MAIN(TokenizerTest)

#include "tst_tokenizertest.moc"
