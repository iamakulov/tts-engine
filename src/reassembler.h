#ifndef REASSEMBLER_H
#define REASSEMBLER_H

#include "collections/List.h"
#include "structures/token.h"
#include "structures/transformationrule.h"

class Reassembler
{
public:
    Reassembler(List<TransformationRule> rules);

    List<Token> reassemble(List<Token> data) const;

private:
    List<TransformationRule> m_rules;
};

#endif // REASSEMBLER_H
