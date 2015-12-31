#include <QString>
#include <QtTest>

#include "../../src/collections/List.h"
#include "../../src/structures/soundtoaudiorule.cpp"
#include "../../src/structures/token.cpp"
#include "../../src/structures/audiodefinition.cpp"
#include "../../src/structures/transformationrule.cpp"
#include "../../src/transformer.cpp"
#include "../../src/audioconvertor.cpp"

Q_DECLARE_METATYPE_TEMPLATE_1ARG(List)
Q_DECLARE_METATYPE(Token)
Q_DECLARE_METATYPE(SoundToAudioRule)
Q_DECLARE_METATYPE(AudioDefinition)

class AudioConvertorTest : public QObject
{
    Q_OBJECT

public:
    AudioConvertorTest();

private Q_SLOTS:
    void convert_data();
    void convert();
};

AudioConvertorTest::AudioConvertorTest()
{
}

void AudioConvertorTest::convert_data()
{
    QTest::addColumn<List<Token>>("passedSounds");
    QTest::addColumn<List<SoundToAudioRule>>("rules");
    QTest::addColumn<List<AudioDefinition>>("convertedAudios");

    List<Token> _;  // An empty placeholder

    QTest::newRow("Complete match")
        << List<Token> {
            Token(TokenName::SOUND, "th"),
            Token(TokenName::SOUND, "i"),
            Token(TokenName::SOUND, "s")
        }
        << List<SoundToAudioRule> {
            SoundToAudioRule(
                List<Token> { Token(TokenName::SOUND, "th") },
                AudioDefinition("audio.wav", 0, 100)
            ),
            SoundToAudioRule(
                List<Token> { Token(TokenName::SOUND, "i") },
                AudioDefinition("audio.wav", 100, 200)
            ),
            SoundToAudioRule(
                List<Token> { Token(TokenName::SOUND, "s") },
                AudioDefinition("audio.wav", 200, 300)
            )
        }
        << List<AudioDefinition> {
            AudioDefinition("audio.wav", 0, 100),
            AudioDefinition("audio.wav", 100, 200),
            AudioDefinition("audio.wav", 200, 300)
        };

    QTest::newRow("Extra intercepting rules")
        << List<Token> {
            Token(TokenName::SOUND, "th"),
            Token(TokenName::SOUND, "i"),
            Token(TokenName::SOUND, "s")
        }
        << List<SoundToAudioRule> {
            SoundToAudioRule(
                List<Token> { Token(TokenName::SOUND, "th") },
                AudioDefinition("audio.wav", 0, 100)
            ),
            SoundToAudioRule(
                List<Token> { Token(TokenName::SOUND, "th") },
                AudioDefinition("audio.wav", 500, 600)
            ),
            SoundToAudioRule(
                List<Token> { Token(TokenName::SOUND, "i") },
                AudioDefinition("audio.wav", 100, 200)
            ),
            SoundToAudioRule(
                List<Token> { Token(TokenName::SOUND, "s") },
                AudioDefinition("audio.wav", 200, 300)
            )
        }
        << List<AudioDefinition> {
            AudioDefinition("audio.wav", 0, 100),
            AudioDefinition("audio.wav", 100, 200),
            AudioDefinition("audio.wav", 200, 300)
        };

    QTest::newRow("Extra sounds")
        << List<Token> {
            Token(TokenName::SOUND, "th"),
            Token(TokenName::SOUND, "i"),
            Token(TokenName::SOUND, "s")
        }
        << List<SoundToAudioRule> {
            SoundToAudioRule(
                List<Token> { Token(TokenName::SOUND, "i") },
                AudioDefinition("audio.wav", 100, 200)
            ),
            SoundToAudioRule(
                List<Token> { Token(TokenName::SOUND, "s") },
                AudioDefinition("audio.wav", 200, 300)
            )
        }
        << List<AudioDefinition> {
            AudioDefinition("audio.wav", 100, 200),
            AudioDefinition("audio.wav", 200, 300)
        };
}

void AudioConvertorTest::convert()
{
    QFETCH(List<Token>, passedSounds);
    QFETCH(List<SoundToAudioRule>, rules);
    QFETCH(List<AudioDefinition>, convertedAudios);

    QCOMPARE(AudioConvertor(rules).convert(passedSounds), convertedAudios);
}

QTEST_APPLESS_MAIN(AudioConvertorTest)

#include "tst_audioconvertortest.moc"
