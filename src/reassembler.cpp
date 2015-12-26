#include "reassembler.h"
#include "transformer.h"

Reassembler::Reassembler(List<TransformationRule> rules)
    : m_rules(rules)
{

}

List<Token> Reassembler::reassemble(List<Token> data) const
{
    return Transformer(m_rules).transform(data);
}

