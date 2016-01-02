#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "collections/List.h"
#include "structures/tokendefinition.h"
#include "structures/transformationrule.h"
#include "structures/ltsrule.h"
#include "structures/soundtoaudiorule.h"
#include "player.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = 0);

    void setTokenDefinitions(List<TokenDefinition> tokenDefinitions);
    void setReassemblerRules(List<TransformationRule> reassemblerRules);
    void setTranscriptionRules(List<TransformationRule> transcriptionRules);
    void setLtsRules(List<LtsRule> ltsRules);
    void setSoundToAudioRules(List<SoundToAudioRule> soundToAudioRules);

public slots:
    void speakText(const QString &text) const;

private /*members*/:
    List<TokenDefinition> m_tokenDefinitions;
    List<TransformationRule> m_reassemblerRules;
    List<TransformationRule> m_transcriptionRules;
    List<LtsRule> m_ltsRules;
    List<SoundToAudioRule> m_soundToAudioRules;

    Player m_player;
};

#endif // CONTROLLER_H
