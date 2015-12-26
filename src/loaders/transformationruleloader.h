#ifndef TRANSFORMATIONRULELOADER_H
#define TRANSFORMATIONRULELOADER_H

#include <QScriptEngine>
#include <QTextStream>
#include <QPointer>
#include "../collections/List.h"
#include "../structures/transformationrule.h"

class TransformationRuleLoader
{
public:
    TransformationRuleLoader(QTextStream* inputStream);

    List<TransformationRule> getData();

private /*methods*/:
    bool reportDataErrors(const QScriptValue &value);

    bool reportHandlerResultError(const QScriptValue &handler, const QScriptValue &result) const;
    bool checkTokenObjectValidity(const QScriptValue &tokenObject) const;

    List<QScriptValue> convertArrayToList(QScriptValue jsArray) const;

    QScriptValue convertTokenToObject(Token token) const;
    Token convertObjectToToken(QScriptValue object) const;

    static void resetEngineState();

private /*members*/:
    QTextStream* m_inputStream;

    // Declaring engine as a static property to prevent it from being destroyed with a TransformationRuleLoader instance.
    // It is required as the engine is used in lambdas passed outside of the class.
    // The pointer is required to delay the engine initialization till the first constructor call,
    // because the engine must only be initialized after Q(Core)Application.
    static QPointer<QScriptEngine> m_engine;
};

#endif // TRANSFORMATIONRULELOADER_H
