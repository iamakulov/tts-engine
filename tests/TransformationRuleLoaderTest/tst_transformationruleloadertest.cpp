#include <QString>
#include <QtTest>

#include "../../src/collections/List.h"
#include "../../src/structures/token.cpp"
#include "../../src/structures/transformationrule.cpp"
#include "../../src/transformer.cpp"
#include "../../src/loaders/transformationruleloader.cpp"

Q_DECLARE_METATYPE_TEMPLATE_1ARG(List)
Q_DECLARE_METATYPE(Token)

class TransformationRuleLoaderTest : public QObject
{
    Q_OBJECT

public:
    TransformationRuleLoaderTest();

private Q_SLOTS:
    void getData_data();
    void getData();
};

TransformationRuleLoaderTest::TransformationRuleLoaderTest()
{
}

void TransformationRuleLoaderTest::getData_data()
{
    QTest::addColumn<QString>("inputText");
    QTest::addColumn<List<Token>>("tokens");
    QTest::addColumn<List<Token>>("transformationResult");

    const auto inputTokens = List<Token> {
        Token("number", "27"),
        Token("space", " "),
        Token("number", "586")
    };

    QTest::newRow("Valid rule")
        << QString(
            "["
            "   {"
            "       matchingSequence: ["
            "           function (token) { return token.name === 'number' },"
            "           function (token) { return token.name === 'space' },"
            "           function (token) { return token.name === 'number' }"
            "       ],"
            "       handler: function (tokens) {"
            "           return [{"
            "               name: 'number',"
            "               data: tokens[0].data + tokens[2].data"
            "           }]"
            "       }"
            "   }"
            "]"
        )
        << inputTokens
        << List<Token> {
               Token("number", "27586"),
           };

    QTest::newRow("Incorrect file format")
        << QString(
            "{"
            "   matchingSequence: ["
            "       function (token) { return token.name === 'number' },"
            "       function (token) { return token.name === 'space' },"
            "       function (token) { return token.name === 'number' }"
            "   ],"
            "   handler: function (tokens) {"
            "       return [{"
            "           name: 'number',"
            "           data: tokens[0].data + tokens[2].data"
            "       }]"
            "   }"
            "}"
        )
        << inputTokens
        << inputTokens;

    QTest::newRow("Incorrect handler result")
        << QString(
           "["
           "   {"
           "       matchingSequence: ["
           "           function (token) { return token.name === 'number' },"
           "           function (token) { return token.name === 'space' },"
           "           function (token) { return token.name === 'number' }"
           "       ],"
           "       handler: function (tokens) {"
           "           return {"
           "               name: 'number',"
           "               data: tokens[0].data + tokens[2].data"
           "           }"
           "       }"
           "   }"
           "]"
        )
        << inputTokens
        << List<Token>();
}

void TransformationRuleLoaderTest::getData()
{
    // There’s no way to compare functions directly.
    // Therefore, to check that rules are parsed properly, the list of them is passed to Transformer,
    // and the returned result is compared with the expected one.
    // This assumes that Transformer is already tested and works properly.

    QFETCH(QString, inputText);
    QFETCH(List<Token>, tokens);
    QFETCH(List<Token>, transformationResult);

    QSharedPointer<QTextStream> inputStream(new QTextStream(inputText.toUtf8()));
    List<TransformationRule> rules = TransformationRuleLoader(inputStream).getData();

    QCOMPARE(Transformer(rules).transform(tokens), transformationResult);
}

QTEST_GUILESS_MAIN(TransformationRuleLoaderTest)

#include "tst_transformationruleloadertest.moc"
