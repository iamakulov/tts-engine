#include "tokendefinitionloader.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QtDebug>

TokenDefinitionLoader::TokenDefinitionLoader(QTextStream *inputStream)
    : m_inputStream(inputStream)
{

}

List<TokenDefinition> TokenDefinitionLoader::getData()
{
    QByteArray textData = m_inputStream->readAll().toUtf8();
    QJsonDocument json = QJsonDocument::fromJson(textData);

    if (!json.isArray()) {
        qWarning() << "TokenDefinitionLoader#getDefinitions: the passed text stream cannot be converted to a JSON array.";
        qWarning() << "The retrieved value:" << textData;
        return List<TokenDefinition>();
    }

    return List<QVariant>(json.array().toVariantList())
        .mappedTo<TokenDefinition>([](const QVariant &arrayElement) {
            QVariantMap object = arrayElement.toMap();

            if (!(object.contains("name") && object.contains("pattern"))) {
                qWarning() << "TokenDefinitionLoader#getDefinitions: an array element is not an object or does not contain 'name' or 'pattern' keys.";
                qWarning() << "The parsed value keys:" << object.keys();

                return TokenDefinition();
            }

            return TokenDefinition(
                object["name"].toString(),
                object["pattern"].toString()
            );
        })
        .filtered([](const TokenDefinition &definition) {
            return !(definition.name().isNull() || definition.pattern().isNull());
        });
}

