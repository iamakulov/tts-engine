#ifndef APP_H
#define APP_H

#include <QApplication>
#include "collections/List.h"
#include "structures/tokendefinition.h"
#include "structures/transformationrule.h"
#include "structures/ltsrule.h"
#include "structures/soundtoaudiorule.h"
#include <iostream>

class App
{
public:
    App(int &argc, char **argv);

public slots:
    int exec();

private /*methods*/:
    QMap<QString, QString> loadSettings() const;

    QSharedPointer<QTextStream> getTextStream(QString path);

    List<TokenDefinition> loadTokenDefinitions(QString path);
    List<TransformationRule> loadTransformationRules(QString path);
    List<LtsRule> loadLtsRules(QString path);
    List<SoundToAudioRule> loadSoundToAudioRules(QString path);

private /*members*/:
    QApplication m_instance;
};

#endif // APP_H
