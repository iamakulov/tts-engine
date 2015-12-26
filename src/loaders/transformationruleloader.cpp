#include "transformationruleloader.h"
#include <QVariant>
#include <QtDebug>
#include <functional>
#include "../structures/token.h"

QPointer<QScriptEngine> TransformationRuleLoader::m_engine = nullptr;

TransformationRuleLoader::TransformationRuleLoader(QTextStream *inputStream)
    : m_inputStream(inputStream)
{
    if (m_engine == nullptr) {
        m_engine = new QScriptEngine();
    }
}

/**
 * @brief Get the list of transformation rules from the text stream passed to the constructor.
 */
List<TransformationRule> TransformationRuleLoader::getData()
{
    resetEngineState();

    QString textData = m_inputStream->readAll();
    QScriptValue value = m_engine->evaluate(textData);

    bool isDataValid = reportDataErrors(value);
    if (!isDataValid) {
        return List<TransformationRule>();
    }

    return convertArrayToList(value)
        .mappedTo<TransformationRule>([this](const QScriptValue &object) {
            // TransformationRule has a { matchingSequence: MatchingSequenceCallback[], handler: HandlerCallback } signature
            // MatchingSequenceCallback is a `Token => Boolean` function
            // HandlerCallback is a `Token[] => Token[]` function

            // Convert a JavaScript-encoded matching sequence to C++
            auto matchingSequence = convertArrayToList(object.property("matchingSequence"))
                // ...This wraps each JavaScript function with a C++ lambda
                .mappedTo<std::function<bool(Token)>>([this](const QScriptValue &tokenMatcher) {
                    return [this, tokenMatcher](Token token) {
                        QScriptValue m(tokenMatcher); // Copy the validator to prevent the SEGFAULT

                        QScriptValue tokenObject = convertTokenToObject(token);
                        QScriptValueList args { tokenObject };

                        return m.call(QScriptValue(), args).toBool();
                    };
                });

            // Convert a JavaScript-encoded handler to C++
            // ...Actually, this wraps a JavaScript function with a C++ lambda
            auto handler = [this, object](List<Token> tokenList) {
                // ...Convert C++ token list to its JavaScript representation
                QScriptValue tokenArray = tokenList
                    // ...Convert each token to its JavaScript representation
                    .mappedTo<QScriptValue>([this](Token token) {
                        return convertTokenToObject(token);
                    })
                    // ...Reduce a list of JavaScript objects to JavaScript array
                    .reducedTo<QScriptValue>([](QScriptValue collector, QScriptValue currentTokenObject, int index) {
                        collector.setProperty(index, currentTokenObject);
                        return collector;
                    }, m_engine->newArray(tokenList.length()));

                // ...Call the method and save the result
                QScriptValueList args { tokenArray };
                QScriptValue handler = object.property("handler");
                QScriptValue transformedTokenArray = handler.call(QScriptValue(), args);

                reportHandlerResultError(handler, transformedTokenArray); // A runtime check that cannot be performed in reportDataErrors()

                // ...Convert the result back to C++ representation
                return convertArrayToList(transformedTokenArray)
                    .mappedTo<Token>([this](QScriptValue object) {
                        checkTokenObjectValidity(object);
                        return convertObjectToToken(object);
                    });
            };

            return TransformationRule(matchingSequence, handler);
        });
}

/**
 * @brief Check if the passed file content is valid, print warnings if it is invalid, and return true or false depending on its validity state.
 */
bool TransformationRuleLoader::reportDataErrors(const QScriptValue &value)
{
    if (!value.isArray()) {
        qWarning() << "TransformationRuleLoader#reportDataErrors: the passed text stream cannot be evaluated as an array.";
        qWarning() << "The evaluated value:" << value.toString();
        return false;
    }

    List<QScriptValue> allItems = convertArrayToList(value);
    List<QScriptValue> validItems = allItems
        .filtered([this](const QScriptValue &object) {
            // Check the matching sequence validity
            QScriptValue matchingSequence = object.property("matchingSequence");
            if (!matchingSequence.isArray()) {
                qWarning() << "TransformationRuleLoader#reportDataErrors: the matchingSequence key is not an array.";
                qWarning() << "The parsed value:" << matchingSequence.toString();
                return false;
            }

            auto allItems = convertArrayToList(object.property("matchingSequence"));
            auto validMatchingSequenceItems = allItems
                .filtered([](const QScriptValue &tokenValidator) {
                    return tokenValidator.isFunction();
                });

            if (validMatchingSequenceItems.length() != allItems.length()) {
                qWarning() << "TransformationRuleLoader#reportDataErrors:" << allItems.length() - validMatchingSequenceItems.length()
                           << "mathcingSequence items are not functions.";
                qWarning() << "The parsed matchingSequence value:" << matchingSequence.toString();
                return false;
            }

            // Check the handler validity
            QScriptValue handler = object.property("handler");
            if (!handler.isFunction()) {
                qWarning() << "TransformationRuleLoader#reportDataErrors: the handler is not a function.";
                qWarning() << "The parsed value:" << handler.toString();
                return false;
            }

            return true;
        });

    if (validItems.length() != allItems.length()) {
        return false;
    }

    if (m_engine->hasUncaughtException()) {
        qWarning() << "TransformationRuleLoader#reportDataErrors: an exception occurred while parsing the passed text stream.";
        qWarning() << "The exception:" << m_engine->uncaughtException().toString();
        qWarning() << "The exception line number:" << m_engine->uncaughtExceptionLineNumber();
        return false;
    }

    return true;
}

/**
 * @brief Check that the TransformationRule handler returned an array.
 * The check cannot be performed in reportDataErrors() as it deals with the function call result.
 * @return True, if the result is valid, or false otherwise.
 */
bool TransformationRuleLoader::reportHandlerResultError(const QScriptValue &handler, const QScriptValue &result) const
{
    if (!result.isArray()) {
        qWarning() << "TransformationRuleLoader#reportHandlerResultError: the result of the JavaScript handler call is not an array.";
        qWarning() << "The handler:" << handler.toString();
        qWarning() << "The result:" << result.toString();
        return false;
    }

    auto allItems = convertArrayToList(result);
    auto validItems = allItems.filtered([this](QScriptValue object) {
        bool isTokenValid = object.isObject() && object.property("name").isString() && object.property("data").isString();
        if (!isTokenValid) {
            qWarning() << "TransformationRuleLoader#reportHandlerResultError: a JavaScript token object is ill-formed.";
            qWarning() << "Either the value is not an object or `name` and/or `data` keys are absent.";
            qWarning() << "The value:" << object.toString();
            qWarning() << "The name key:" << object.property("name").toString();
            qWarning() << "The data key:" << object.property("data").toString();
            return false;
        }

        return true;
    });

    if (validItems.length() != allItems.length()) {
        return false;
    }

    return true;
}

/**
 * @brief Check that the passed object is valid token.
 * The check cannot be performed in reportDataErrors() as it deals with the function call result.
 * @return True, if the result is valid, or false otherwise.
 */
bool TransformationRuleLoader::checkTokenObjectValidity(const QScriptValue &tokenObject) const
{
    bool isTokenValid = tokenObject.isObject() && tokenObject.property("name").isString() && tokenObject.property("data").isString();
    if (!isTokenValid) {
        qWarning() << "TransformationRuleLoader#checkTokenObjectValidity: a JavaScript token object is ill-formed.";
        qWarning() << "Either the value is not an object or `name` and/or `data` keys are absent.";
        qWarning() << "The value:" << tokenObject.toString();
        qWarning() << "The name key:" << tokenObject.property("name").toString();
        qWarning() << "The data key:" << tokenObject.property("data").toString();
        return false;
    }

    return true;
}

/**
 * @brief Convert a JavaScript array to a list of QScriptValues.
 */
List<QScriptValue> TransformationRuleLoader::convertArrayToList(QScriptValue jsArray) const
{
    List<QScriptValue> result;

    int length = jsArray.property("length").toInteger();
    for (int i = 0; i < length; ++i) {
        result.append(jsArray.property(i));
    }

    return result;
}

/**
 * @brief Convert a Token to an identical JavaScript object.
 */
QScriptValue TransformationRuleLoader::convertTokenToObject(Token token) const
{
    QScriptValue jsObject = m_engine->newObject();
    jsObject.setProperty("name", token.name());
    jsObject.setProperty("data", token.data());
    return jsObject;
}

/**
 * @brief Convert a JavaScript object to an identical object.
 */
Token TransformationRuleLoader::convertObjectToToken(QScriptValue object) const
{
    QString name = object.property("name").toString();
    QString data = object.property("data").toString();
    return Token(name, data);
}

void TransformationRuleLoader::resetEngineState()
{
    m_engine->clearExceptions();
}

