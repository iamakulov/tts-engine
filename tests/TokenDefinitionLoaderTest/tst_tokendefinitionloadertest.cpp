#include <QString>
#include <QtTest>

#include "../../src/collections/List.h"
#include "../../src/structures/tokendefinition.cpp"
#include "../../src/loaders/tokendefinitionloader.cpp"

Q_DECLARE_METATYPE_TEMPLATE_1ARG(List)
Q_DECLARE_METATYPE(TokenDefinition)


class TokenDefinitionLoaderTest : public QObject
{
    Q_OBJECT

public:
    TokenDefinitionLoaderTest();

private Q_SLOTS:
    void getDefinitions_data();
    void getDefinitions();
};

TokenDefinitionLoaderTest::TokenDefinitionLoaderTest()
{
}

void TokenDefinitionLoaderTest::getDefinitions_data()
{
    QTest::addColumn<QString>("inputText");
    QTest::addColumn<List<TokenDefinition>>("parsedTokens");

    QTest::newRow("Valid tokens")
        << QString("[{'name': 'word', 'pattern': '\\\\w+'}, {'name': 'space', 'pattern': '\\\\s+'}]").replace("'", "\"")
        << List<TokenDefinition>{
           TokenDefinition("word", "\\w+"),
           TokenDefinition("space", "\\s+")
        };

    QTest::newRow("Invalid file structure")
        << QString("{'name': 'word', 'pattern': '\\\\w+'}").replace("'", "\"")
        << List<TokenDefinition>();

    QTest::newRow("Invalid token structure")
        << QString("[{'name': 'word', 'pattern': '\\\\w+'}, {'name': 'space', 'value': '\\\\s+'}]").replace("'", "\"")
        << List<TokenDefinition>();
}

void TokenDefinitionLoaderTest::getDefinitions()
{
    QFETCH(QString, inputText);
    QFETCH(List<TokenDefinition>, parsedTokens);

    QSharedPointer<QTextStream> stream(new QTextStream(&inputText));

    QCOMPARE(TokenDefinitionLoader(stream).getData(), parsedTokens);
}

QTEST_APPLESS_MAIN(TokenDefinitionLoaderTest)

#include "tst_tokendefinitionloadertest.moc"
