#include <QString>
#include <QtTest>

#include "../../src/collections/List.h"
#include "../../src/structures/transformationrule.cpp"
#include "../../src/structures/token.cpp"
#include "../../src/transformer.cpp"
#include "../../src/transcriber.cpp"

Q_DECLARE_METATYPE_TEMPLATE_1ARG(List)
Q_DECLARE_METATYPE(TransformationRule)
Q_DECLARE_METATYPE(Token)

class TranscriberTest : public QObject
{
    Q_OBJECT

public:
    TranscriberTest();

private Q_SLOTS:
    void transcribe_data();
    void transcribe();
};

TranscriberTest::TranscriberTest()
{
}

void TranscriberTest::transcribe_data()
{
    QTest::addColumn<List<Token>>("passedTokens");
    QTest::addColumn<List<TransformationRule>>("rules");
    QTest::addColumn<List<Token>>("transcribedTokens");

    auto isPoint = [](Token token) {
        return token.name() == "punctuation" && token.data() == ".";
    };

    QTest::newRow("Simple transcription")
        << List<Token> {
            Token(TokenName::WORD, "A"),
            Token(TokenName::WORD, "quick"),
            Token(TokenName::WORD, "fox"),
            Token("punctuation", ".")
        }
        << List<TransformationRule> {
            TransformationRule(List<std::function<bool(Token)>>{ isPoint }, [](List<Token> tokens) {
                Q_UNUSED(tokens);
                return List<Token> { Token(TokenName::WORD, "point") };
            })
        }
        << List<Token> {
            Token(TokenName::WORD, "A"),
            Token(TokenName::WORD, "quick"),
            Token(TokenName::WORD, "fox"),
            Token(TokenName::WORD, "point")
        };

    QTest::newRow("Removing non-word tokens")
        << List<Token> {
            Token(TokenName::WORD, "A"),
            Token(TokenName::WORD, "quick"),
            Token(TokenName::WORD, "fox"),
            Token("punctuation", ".")
        }
        << List<TransformationRule> { }
        << List<Token> {
            Token(TokenName::WORD, "A"),
            Token(TokenName::WORD, "quick"),
            Token(TokenName::WORD, "fox")
        };
}

void TranscriberTest::transcribe()
{
    QFETCH(List<Token>, passedTokens);
    QFETCH(List<TransformationRule>, rules);
    QFETCH(List<Token>, transcribedTokens);
    QCOMPARE(Transcriber(rules).transcribe(passedTokens), transcribedTokens);
}

QTEST_APPLESS_MAIN(TranscriberTest)

#include "tst_transcribertest.moc"
