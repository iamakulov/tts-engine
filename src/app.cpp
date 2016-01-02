#include "app.h"
#include <QFile>
#include <QSettings>
#include <QTextCodec>
#include "controller.h"
#include "view.h"
#include "loaders/tokendefinitionloader.h"
#include "loaders/transformationruleloader.h"
#include "loaders/ltsruleloader.h"
#include "loaders/soundtoaudioruleloader.h"

App::App(int &argc, char **argv)
    : m_instance(argc, argv)
{

}

int App::exec()
{
    QMap<QString, QString> settings = loadSettings();

    Controller controller;
    controller.setTokenDefinitions(loadTokenDefinitions(settings.value("tokenDefinitionsPath")));
    controller.setReassemblerRules(loadTransformationRules(settings.value("reassemblerRulesPath")));
    controller.setTranscriptionRules(loadTransformationRules(settings.value("transcriptionsRulesPath")));
    controller.setLtsRules(loadLtsRules(settings.value("ltsRulesPath")));
    controller.setSoundToAudioRules(loadSoundToAudioRules(settings.value("soundToAudioRulesPath")));

    View view;
    QObject::connect(&view, &View::textSubmitted, &controller, &Controller::speakText);

    view.show();

    return m_instance.exec();
}

QMap<QString, QString> App::loadSettings() const
{
    QSettings settings("TTS Engine", "Ivan Akulov");

    return (List<QPair<QString, QString>> {
        // Key & default value pairs
        qMakePair(QString("tokenDefinitionsPath"), QString("definitions/tokens.json")),
        qMakePair(QString("reassemblerRulesPath"), QString("rules/reassembler.js")),
        qMakePair(QString("transcriptionsRulesPath"), QString("rules/transcription.js")),
        qMakePair(QString("ltsRulesPath"), QString("rules/lts.json")),
        qMakePair(QString("soundToAudioRulesPath"), QString("rules/soundToAudio.json"))
    }).reducedTo<QMap<QString, QString>>([&settings](auto map, auto pair) {
        auto key = pair.first;
        auto defaultValue = pair.second;
        map[key] = settings.value(key, defaultValue).toString();
        return map;
    }, QMap<QString, QString>());
}

QSharedPointer<QTextStream> App::getTextStream(QString path)
{
    QFile *file = new QFile(path, &m_instance);

    QSharedPointer<QTextStream> pointer;
    if (file->open(QFile::ReadOnly)) {
        pointer.reset(new QTextStream(file));
    } else {
        qWarning() << "Cannot open the file '" + path + "'";
        pointer.reset(new QTextStream);
    }

    pointer->setCodec(QTextCodec::codecForName("UTF-8"));

    return pointer;
}

List<TokenDefinition> App::loadTokenDefinitions(QString path)
{
    QSharedPointer<QTextStream> stream = getTextStream(path);
    return TokenDefinitionLoader(stream).getData();
}

List<TransformationRule> App::loadTransformationRules(QString path)
{
    QSharedPointer<QTextStream> stream = getTextStream(path);
    return TransformationRuleLoader(stream).getData();
}

List<LtsRule> App::loadLtsRules(QString path)
{
    QSharedPointer<QTextStream> stream = getTextStream(path);
    return LtsRuleLoader(stream).getData();
}

List<SoundToAudioRule> App::loadSoundToAudioRules(QString path)
{
    QSharedPointer<QTextStream> stream = getTextStream(path);
    return SoundToAudioRuleLoader(stream).getData();
}
