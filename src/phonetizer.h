#ifndef PHONETIZER_H
#define PHONETIZER_H

#include "collections/List.h"
#include "structures/ltsrule.h"

class Phonetizer
{
public:
    Phonetizer(List<LtsRule> rules);

    List<Token> phonetize(List<Token> letters) const;

private:
    List<LtsRule> m_rules;
};

#endif // PHONETIZER_H
