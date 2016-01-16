#include "transcriber.h"
#include "transformer.h"

Transcriber::Transcriber(List<TransformationRule> rules)
    : m_rules(rules)
{

}

List<Token> Transcriber::transcribe(List<Token> data) const
{
    return Transformer(m_rules).transform(data)
        .filtered([](Token token) {
            return token.name() == TokenName::WORD;
        });
}

