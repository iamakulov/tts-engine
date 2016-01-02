#include <QString>
#include <QtTest>

#include "../../src/collections/List.h"
#include "../../src/structures/ltsrule.cpp"
#include "../../src/structures/token.cpp"
#include "../../src/loaders/ltsruleloader.cpp"

Q_DECLARE_METATYPE_TEMPLATE_1ARG(List)
Q_DECLARE_METATYPE(LtsRule)
Q_DECLARE_METATYPE(Token)

class LtsRuleLoaderTest : public QObject
{
    Q_OBJECT

public:
    LtsRuleLoaderTest();

private Q_SLOTS:
    void getData_data();
    void getData();
};

LtsRuleLoaderTest::LtsRuleLoaderTest()
{
}

void LtsRuleLoaderTest::getData_data()
{
    QTest::addColumn<QString>("inputText");
    QTest::addColumn<List<LtsRule>>("rules");

    QTest::newRow("Plain rules")
        << QString("[\"[a] = a\", \"[b] = b\", \"[c] = c\"]")
        << List<LtsRule> {
            LtsRule(List<Token>(), List<Token> { Token(TokenName::LETTER, "a") }, List<Token>(), List<Token>{ Token(TokenName::SOUND, "a") }),
            LtsRule(List<Token>(), List<Token> { Token(TokenName::LETTER, "b") }, List<Token>(), List<Token>{ Token(TokenName::SOUND, "b") }),
            LtsRule(List<Token>(), List<Token> { Token(TokenName::LETTER, "c") }, List<Token>(), List<Token>{ Token(TokenName::SOUND, "c") })
        };
}

void LtsRuleLoaderTest::getData()
{
    QFETCH(QString, inputText);
    QFETCH(List<LtsRule>, rules);

    QSharedPointer<QTextStream> stream(new QTextStream(&inputText));

    QCOMPARE(LtsRuleLoader(stream).getData(), rules);
}

QTEST_APPLESS_MAIN(LtsRuleLoaderTest)

#include "tst_ltsruleloadertest.moc"
