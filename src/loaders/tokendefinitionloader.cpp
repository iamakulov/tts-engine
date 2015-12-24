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
    QString textData = m_inputStream->readAll();
    QJsonDocument json = QJsonDocument::fromJson(textData.toUtf8());

    bool isDataValid = reportDataErrors(json);
    if (!isDataValid) {
        return List<TokenDefinition>();
    }

    return List<QVariant>(json.array().toVariantList())
        .mappedTo<TokenDefinition>([](const QVariant &arrayElement) {
            QVariantMap object = arrayElement.toMap();

            return TokenDefinition(
                object["name"].toString(),
                object["pattern"].toString()
            );
        });
}

bool TokenDefinitionLoader::reportDataErrors(const QJsonDocument &json) const
{
    if (!json.isArray()) {
        qWarning() << "TokenDefinitionLoader#getData: the passed text stream cannot be converted to a JSON array.";
        qWarning() << "The parsed JSON content:" << json.toJson();
        return false;
    }

    List<QVariant> allItems = json.array().toVariantList();
    List<QVariant> validItems = allItems
        .filtered([](const QVariant &arrayElement) {
            QVariantMap object = arrayElement.toMap();

            if (!(object.contains("name") && object.contains("pattern"))) {
                qWarning() << "TokenDefinitionLoader#getDefinitions: an array element is not an object or does not contain 'name' or 'pattern' keys.";
                qWarning() << "The parsed value keys:" << object.keys();
                return false;
            }

            if (object["name"].userType() != QVariant::String) {
                qWarning() << "TokenDefinitionLoader#getDefinitions: the 'name' value of the array element is not a string.";
                qWarning() << "The parsed value keys:" << object.keys();
                qWarning() << "The value type:" << object["name"].userType();
                return false;
            }

            if (object["pattern"].userType() != QVariant::String) {
                qWarning() << "TokenDefinitionLoader#getDefinitions: the 'name' value of the array element is not a string.";
                qWarning() << "The parsed value keys:" << object.keys();
                qWarning() << "The value type:" << object["pattern"].userType();
                return false;
            }

            return true;
        });

    if (validItems.length() != allItems.length()) {
        return false;
    }

    return true;
}
