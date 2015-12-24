#ifndef TOKENDEFINITIONLOADER_H
#define TOKENDEFINITIONLOADER_H

#include <QTextStream>
#include "../collections/List.h"
#include "../structures/tokendefinition.h"

class TokenDefinitionLoader
{
public:
    TokenDefinitionLoader(QTextStream* inputStream);

    List<TokenDefinition> getData();

private /*members*/:
    QTextStream* m_inputStream;
};

#endif // TOKENDEFINITIONLOADER_H
