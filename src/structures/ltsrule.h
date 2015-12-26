#ifndef LTSRULE_H
#define LTSRULE_H

#include "../collections/List.h"
#include "token.h"

class LtsRule
{
public:
    LtsRule(const List<Token> &previousContext = List<Token>(),
            const List<Token> &target = List<Token>(),
            const List<Token> &nextContext = List<Token>(),
            const List<Token> &sounds = List<Token>());

    List<Token> previousContext() const;
    List<Token> target() const;
    List<Token> nextContext() const;
    List<Token> sounds() const;

    bool operator==(const LtsRule &other) const;

private:
    List<Token> m_previousContext;
    List<Token> m_target;
    List<Token> m_nextContext;
    List<Token> m_sounds;
};

#endif // LTSRULE_H
