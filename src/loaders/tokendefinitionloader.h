#ifndef TOKENDEFINITIONLOADER_H
#define TOKENDEFINITIONLOADER_H

#include <QSharedPointer>
#include <QTextStream>
#include "../collections/List.h"
#include "../structures/tokendefinition.h"

class TokenDefinitionLoader
{
public:
    TokenDefinitionLoader(QSharedPointer<QTextStream> inputStream);

    List<TokenDefinition> getData();

private /*methods*/:
    bool reportDataErrors(const QJsonDocument &json) const;

private /*members*/:
    QSharedPointer<QTextStream> m_inputStream;
};

#endif // TOKENDEFINITIONLOADER_H
