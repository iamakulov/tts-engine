#include <QString>
#include <QtTest>

#include "../../src/collections/List.h"
#include "../../src/structures/transformationrule.cpp"
#include "../../src/structures/token.cpp"
#include "../../src/transformer.cpp"

Q_DECLARE_METATYPE_TEMPLATE_1ARG(List)
Q_DECLARE_METATYPE(TransformationRule)
Q_DECLARE_METATYPE(Token)

class TransformerTest : public QObject
{
    Q_OBJECT

public:
    TransformerTest();

private Q_SLOTS:
    void transform_data();
    void transform();
};

TransformerTest::TransformerTest()
{
}

void TransformerTest::transform_data()
{
    QTest::addColumn<List<Token>>("passedTokens");
    QTest::addColumn<List<TransformationRule>>("rules");
    QTest::addColumn<List<Token>>("transformedTokens");

    auto isNumber = [](Token token) {
        return token.name() == "number";
    };

    auto isSpace = [](Token token) {
        return token.name() == "space";
    };

    auto isWord = [](Token token) {
        return token.name() == "word";
    };

    auto isPunctuation = [](Token token) {
        return token.name() == "punctuation";
    };

    QTest::newRow("One rule matching all tokens")
        << List<Token> {
            Token("number", "27"),
            Token("space", " "),
            Token("number", "586")
        }
        << List<TransformationRule> {
            TransformationRule(List<std::function<bool(Token)>>{ isNumber, isSpace, isNumber }, [](List<Token> tokens) {
                return List<Token> { Token("number", tokens[0].data() + tokens[2].data()) };
            })
        }
        << List<Token> {
            Token("number", "27586")
        };

    QTest::newRow("One rule matching a part of tokens")
        << List<Token> {
            Token("word", "Mary"),
            Token("space", " "),
            Token("word", "had"),
            Token("space", " "),
            Token("number", "27"),
            Token("space", " "),
            Token("number", "586"),
            Token("space", " "),
            Token("word", "sheeps")
        }
        << List<TransformationRule> {
            TransformationRule(List<std::function<bool(Token)>>{ isNumber, isSpace, isNumber }, [](List<Token> tokens) {
                return List<Token> { Token("number", tokens[0].data() + tokens[2].data()) };
            })
        }
        << List<Token> {
            Token("word", "Mary"),
            Token("space", " "),
            Token("word", "had"),
            Token("space", " "),
            Token("number", "27586"),
            Token("space", " "),
            Token("word", "sheeps")
        };

    QTest::newRow("One rule matching no tokens")
        << List<Token> {
            Token("word", "Mary"),
            Token("space", " "),
            Token("word", "had"),
            Token("space", " "),
            Token("number", "55"),
            Token("space", " "),
            Token("word", "sheeps")
        }
        << List<TransformationRule> {
            TransformationRule(List<std::function<bool(Token)>>{ isNumber, isSpace, isNumber }, [](List<Token> tokens) {
                return List<Token> { Token("number", tokens[0].data() + tokens[2].data()) };
            })
        }
        << List<Token> {
            Token("word", "Mary"),
            Token("space", " "),
            Token("word", "had"),
            Token("space", " "),
            Token("number", "55"),
            Token("space", " "),
            Token("word", "sheeps")
        };

    QTest::newRow("Several rules matching each own token sequence")
        << List<Token> {
            Token("number", "27"),
            Token("space", " "),
            Token("number", "586"),
            Token("space", " "),
            Token("word", "shee"),
            Token("word", "ps")
        }
        << List<TransformationRule> {
            TransformationRule(List<std::function<bool(Token)>>{ isNumber, isSpace, isNumber }, [](List<Token> tokens) {
                return List<Token> { Token("number", tokens[0].data() + tokens[2].data()) };
            }),
            TransformationRule(List<std::function<bool(Token)>>{ isWord, isWord }, [](List<Token> tokens) {
                return List<Token> { Token("word", tokens[0].data() + tokens[1].data()) };
            })
        }
        << List<Token> {
            Token("number", "27586"),
            Token("space", " "),
            Token("word", "sheeps")
        };

    QTest::newRow("Several rules matching one token sequence")
        << List<Token> {
            Token("number", "27"),
            Token("space", " "),
            Token("number", "586")
        }
        << List<TransformationRule> {
            TransformationRule(List<std::function<bool(Token)>>{ isNumber, isSpace, isNumber }, [](List<Token> tokens) {
                return List<Token> { tokens[0], Token("punctuation", ","), tokens[2] };
            }),
            TransformationRule(List<std::function<bool(Token)>>{ isNumber, isPunctuation, isNumber }, [](List<Token> tokens) {
                return List<Token> { Token("number", tokens[0].data() + tokens[2].data()) };
            })
        }
        << List<Token> {
            Token("number", "27586")
        };
}

void TransformerTest::transform()
{
    QFETCH(List<Token>, passedTokens);
    QFETCH(List<TransformationRule>, rules);
    QFETCH(List<Token>, transformedTokens);
    QCOMPARE(Transformer(rules).transform(passedTokens), transformedTokens);
}

QTEST_APPLESS_MAIN(TransformerTest)

#include "tst_transformertest.moc"
