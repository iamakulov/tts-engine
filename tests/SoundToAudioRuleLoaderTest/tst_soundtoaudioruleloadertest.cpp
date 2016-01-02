#include <QString>
#include <QtTest>

#include "../../src/collections/List.h"
#include "../../src/structures/soundtoaudiorule.cpp"
#include "../../src/structures/token.cpp"
#include "../../src/structures/audiodefinition.cpp"
#include "../../src/loaders/soundtoaudioruleloader.cpp"

Q_DECLARE_METATYPE_TEMPLATE_1ARG(List)
Q_DECLARE_METATYPE(SoundToAudioRule)
Q_DECLARE_METATYPE(Token)
Q_DECLARE_METATYPE(AudioDefinition)

class SoundToAudioRuleLoaderTest : public QObject
{
    Q_OBJECT

public:
    SoundToAudioRuleLoaderTest();

private Q_SLOTS:
    void getData_data();
    void getData();
};

SoundToAudioRuleLoaderTest::SoundToAudioRuleLoaderTest()
{
}

void SoundToAudioRuleLoaderTest::getData_data()
{
    QTest::addColumn<QString>("inputText");
    QTest::addColumn<List<SoundToAudioRule>>("rules");

    QTest::newRow("One sound, one audio")
        << QString("{\"a\": \"sounds.wav 100 500\", \"th\": \"sounds.wav 700 1000\"}")
        << List<SoundToAudioRule> {
            SoundToAudioRule(
                List<Token> { Token(TokenName::SOUND, "a") },
                AudioDefinition("sounds.wav", 100, 500)
            ),
            SoundToAudioRule(
                List<Token> { Token(TokenName::SOUND, "th") },
                AudioDefinition("sounds.wav", 700, 1000)
            )
        };

    QTest::newRow("Multiple sounds, one audio")
        << QString("{\"a e\": \"sounds.wav 100 500\", \"th e\": \"sounds.wav 700 1000\"}")
        << List<SoundToAudioRule> {
            SoundToAudioRule(
                List<Token> { Token(TokenName::SOUND, "a"), Token(TokenName::SOUND, "e") },
                AudioDefinition("sounds.wav", 100, 500)
            ),
            SoundToAudioRule(
                List<Token> { Token(TokenName::SOUND, "th"), Token(TokenName::SOUND, "e") },
                AudioDefinition("sounds.wav", 700, 1000)
            )
        };

    QTest::newRow("File name with spaces")
        << QString("{\"a\": \"better sounds.wav 100 500\", \"th\": \"'better sounds.wav' 700 1000\"}")
        << List<SoundToAudioRule> {
            SoundToAudioRule(
               List<Token> { Token(TokenName::SOUND, "a") },
                AudioDefinition("better sounds.wav", 100, 500)
            ),
            SoundToAudioRule(
                List<Token> { Token(TokenName::SOUND, "th") },
                AudioDefinition("'better sounds.wav'", 700, 1000)
            )
        };
}

void SoundToAudioRuleLoaderTest::getData()
{
    QFETCH(QString, inputText);
    QFETCH(List<SoundToAudioRule>, rules);

    QSharedPointer<QTextStream> stream(new QTextStream(&inputText));

    QCOMPARE(SoundToAudioRuleLoader(stream).getData(), rules);
}

QTEST_APPLESS_MAIN(SoundToAudioRuleLoaderTest)

#include "tst_soundtoaudioruleloadertest.moc"
