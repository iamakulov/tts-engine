#ifndef SOUNDTOAUDIORULELOADER_H
#define SOUNDTOAUDIORULELOADER_H

#include <QSharedPointer>
#include <QTextStream>
#include "../collections/List.h"
#include "../structures/soundtoaudiorule.h"

class SoundToAudioRuleLoader
{
public:
    SoundToAudioRuleLoader(QSharedPointer<QTextStream> inputStream);

    List<SoundToAudioRule> getData();

private /*methods*/:
    bool reportDataErrors(const QJsonDocument &json) const;

    bool validateSoundToAudioRule(const QString &sounds, const QString &audio) const;
    SoundToAudioRule parseSoundToAudioRule(const QString &sounds, const QString &audio) const;
    
private /*members*/:
    QSharedPointer<QTextStream> m_inputStream;
};

#endif // SOUNDTOAUDIORULELOADER_H
