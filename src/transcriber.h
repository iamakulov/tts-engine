#ifndef TRANSCRIBER_H
#define TRANSCRIBER_H

#include "collections/List.h"
#include "structures/token.h"
#include "structures/transformationrule.h"

class Transcriber
{
public:
    Transcriber(List<TransformationRule> rules);

    List<Token> transcribe(List<Token> data) const;

private:
    List<TransformationRule> m_rules;
};

#endif // TRANSCRIBER_H
