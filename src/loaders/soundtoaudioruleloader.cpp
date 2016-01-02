#include "soundtoaudioruleloader.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QDebug>

SoundToAudioRuleLoader::SoundToAudioRuleLoader(QSharedPointer<QTextStream> inputStream)
    : m_inputStream(inputStream)
{

}

List<SoundToAudioRule> SoundToAudioRuleLoader::getData()
{
    QString textData = m_inputStream->readAll();
    QJsonDocument json = QJsonDocument::fromJson(textData.toUtf8());

    bool isDataValid = reportDataErrors(json);
    if (!isDataValid) {
        return List<SoundToAudioRule>();
    }

    auto objectMap = json.object().toVariantMap();

    return List<QString>(objectMap.keys())
        // Map to a list of key-value pairs
        .mappedTo<QPair<QString, QString>>([objectMap](const QString &objectKey) {
            return qMakePair(objectKey, objectMap[objectKey].toString());
        })
        .mappedTo<SoundToAudioRule>([this](auto pair){
            // `this` is required due to a gcc bug: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67952
            return this->parseSoundToAudioRule(pair.first, pair.second);
        });
}

bool SoundToAudioRuleLoader::reportDataErrors(const QJsonDocument &json) const
{
    if (!json.isObject()) {
        qWarning() << "SoundToAudioRuleLoader#reportDataErrors: the passed text stream cannot be converted to a JSON object.";
        qWarning() << "The parsed JSON content:" << json.toJson();
        return false;
    }

    auto allItems = json.object().toVariantMap();
    auto validItems = List<QString>(allItems.keys())
        // Map to a list of key-value pairs
        .mappedTo<QPair<QString, QString>>([allItems](const QString &objectKey) {
            return qMakePair(objectKey, allItems[objectKey].toString());
        })
        .filtered([this](auto pair){
            // `this` is required due to a gcc bug: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=67952
            if (!this->validateSoundToAudioRule(pair.first, pair.second)) {
                qWarning() << "SoundToAudioRuleLoader#reportDataErrors: a key-value pair cannot be parsed to a sound-to-audio rule.";
                qWarning() << "The rule must look like '\"a\": \"sounds.wav 100 500\"'.";
                qWarning() << "The parsed pair:" << pair.first << pair.second;
                return false;
            }

            return true;
        });

    if (validItems.size() != allItems.size()) {
        return false;
    }

    return true;
}

bool SoundToAudioRuleLoader::validateSoundToAudioRule(const QString &sounds, const QString &audio) const
{
    return QRegularExpression("^\\S+(\\s+\\S+)*$").match(sounds).hasMatch()
        && QRegularExpression("^.+\\s+\\d+\\s+\\d+$").match(audio).hasMatch();
}

SoundToAudioRule SoundToAudioRuleLoader::parseSoundToAudioRule(const QString &sounds, const QString &audio) const
{
    auto splitSounds = sounds.split(" ", QString::SkipEmptyParts);
    auto parsedSounds = List<QString>(splitSounds).mappedTo<Token>([](auto sound) {
       return Token(TokenName::SOUND, sound);
    });

    auto splitAudio = audio.split(" ");
    auto file = splitAudio.mid(0, splitAudio.length() - 2).join(" ");
    auto startTime = splitAudio[splitAudio.length() - 2].toInt();
    auto endTime = splitAudio[splitAudio.length() - 1].toInt();
    auto parsedAudio = AudioDefinition(file, startTime, endTime);

    return SoundToAudioRule(parsedSounds, parsedAudio);
}
