#include <QString>
#include <QtTest>

#include "../../src/collections/List.h"
#include "../../src/structures/ltsrule.cpp"
#include "../../src/structures/token.cpp"
#include "../../src/structures/transformationrule.cpp"
#include "../../src/transformer.cpp"
#include "../../src/phonetizer.cpp"

Q_DECLARE_METATYPE_TEMPLATE_1ARG(List)
Q_DECLARE_METATYPE(Token)
Q_DECLARE_METATYPE(LtsRule)

class PhonetizerTest : public QObject
{
    Q_OBJECT

public:
    PhonetizerTest();

private Q_SLOTS:
    void phonetize_data();
    void phonetize();
};

PhonetizerTest::PhonetizerTest()
{
}

void PhonetizerTest::phonetize_data()
{
    QTest::addColumn<List<Token>>("passedLetters");
    QTest::addColumn<List<LtsRule>>("rules");
    QTest::addColumn<List<Token>>("phonetizedSounds");

    List<Token> _;  // An empty placeholder

    QTest::newRow("Complete match")
        << List<Token> {
            Token(TokenName::LETTER, "t"),
            Token(TokenName::LETTER, "h"),
            Token(TokenName::LETTER, "i"),
            Token(TokenName::LETTER, "s")
        }
        << List<LtsRule> {
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "t"),
                    Token(TokenName::LETTER, "h")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "th")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "i")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "i")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "s")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "s")
                }
            )
        }
        << List<Token> {
            Token(TokenName::SOUND, "th"),
            Token(TokenName::SOUND, "i"),
            Token(TokenName::SOUND, "s")
        };

    QTest::newRow("Extra non-intercepting rules")
        << List<Token> {
            Token(TokenName::LETTER, "t"),
            Token(TokenName::LETTER, "h"),
            Token(TokenName::LETTER, "i"),
            Token(TokenName::LETTER, "s")
        }
        << List<LtsRule> {
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "t"),
                    Token(TokenName::LETTER, "h")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "th")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "i")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "i")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "s")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "s")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "a")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "a")
                }
            )
        }
        << List<Token> {
            Token(TokenName::SOUND, "th"),
            Token(TokenName::SOUND, "i"),
            Token(TokenName::SOUND, "s")
        };

    QTest::newRow("Extra intercepting rules")
        << List<Token> {
            Token(TokenName::LETTER, "t"),
            Token(TokenName::LETTER, "h"),
            Token(TokenName::LETTER, "i"),
            Token(TokenName::LETTER, "s")
        }
        << List<LtsRule> {
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "t"),
                    Token(TokenName::LETTER, "h")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "th")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "i")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "i")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "s")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "s")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "t")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "t")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "h")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "h")
                }
            )
        }
        << List<Token> {
            Token(TokenName::SOUND, "th"),
            Token(TokenName::SOUND, "i"),
            Token(TokenName::SOUND, "s")
        };

    QTest::newRow("Extra rules with the same target")
        << List<Token> {
            Token(TokenName::LETTER, "t"),
            Token(TokenName::LETTER, "h"),
            Token(TokenName::LETTER, "i"),
            Token(TokenName::LETTER, "s")
        }
        << List<LtsRule> {
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "t"),
                    Token(TokenName::LETTER, "h")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "th")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "i")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "i")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "s")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "s")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "t"),
                    Token(TokenName::LETTER, "h")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "d")
                }
            )
        }
        << List<Token> {
            Token(TokenName::SOUND, "th"),
            Token(TokenName::SOUND, "i"),
            Token(TokenName::SOUND, "s")
        };

    QTest::newRow("Extra letters without matching rules")
        << List<Token> {
            Token(TokenName::LETTER, "t"),
            Token(TokenName::LETTER, "h"),
            Token(TokenName::LETTER, "i"),
            Token(TokenName::LETTER, "s")
        }
        << List<LtsRule> {
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "t"),
                    Token(TokenName::LETTER, "h")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "th")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "i")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "i")
                }
            ),

        }
        << List<Token> {
            Token(TokenName::SOUND, "th"),
            Token(TokenName::SOUND, "i")
        };

    QTest::newRow("Word separators without matching rules")
        << List<Token> {
            Token(TokenName::WORD_SEPARATOR),
            Token(TokenName::LETTER, "t"),
            Token(TokenName::LETTER, "h"),
            Token(TokenName::LETTER, "i"),
            Token(TokenName::LETTER, "s"),
            Token(TokenName::WORD_SEPARATOR)
        }
        << List<LtsRule> {
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "t"),
                    Token(TokenName::LETTER, "h")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "th")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "i")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "i")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "s")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "s")
                }
            )
        }
        << List<Token> {
            Token(TokenName::SOUND, "th"),
            Token(TokenName::SOUND, "i"),
            Token(TokenName::SOUND, "s")
        };

    QTest::newRow("Rules with previous context")
        << List<Token> {
            Token(TokenName::LETTER, "t"),
            Token(TokenName::LETTER, "h"),
            Token(TokenName::LETTER, "i"),
            Token(TokenName::LETTER, "s"),
            Token(TokenName::WORD_SEPARATOR),
            Token(TokenName::LETTER, "i"),
            Token(TokenName::LETTER, "s")
        }
        << List<LtsRule> {
            LtsRule(
                List<Token> {
                    Token(TokenName::LETTER, "t"),
                    Token(TokenName::LETTER, "h")
                },
                List<Token> {
                    Token(TokenName::LETTER, "i"),
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "ii")
                }
            ),
            LtsRule(
                List<Token> {
                    Token(TokenName::WORD_SEPARATOR)
                },
                List<Token> {
                    Token(TokenName::LETTER, "i")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "iii")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "t"),
                    Token(TokenName::LETTER, "h")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "th")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "i")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "i")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "s")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "s")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::WORD_SEPARATOR)
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "_")
                }
            )
        }
        << List<Token> {
            Token(TokenName::SOUND, "th"),
            Token(TokenName::SOUND, "ii"),
            Token(TokenName::SOUND, "s"),
            Token(TokenName::SOUND, "_"),
            Token(TokenName::SOUND, "iii"),
            Token(TokenName::SOUND, "s")
        };

    QTest::newRow("Rules with next context")
        << List<Token> {
            Token(TokenName::LETTER, "t"),
            Token(TokenName::LETTER, "h"),
            Token(TokenName::LETTER, "i"),
            Token(TokenName::LETTER, "s"),
            Token(TokenName::WORD_SEPARATOR),
            Token(TokenName::LETTER, "s"),
            Token(TokenName::LETTER, "i"),
            Token(TokenName::LETTER, "c")
        }
        << List<LtsRule> {
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "s"),
                },
                List<Token> {
                    Token(TokenName::WORD_SEPARATOR)
                },
                List<Token> {
                    Token(TokenName::SOUND, "ss")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "s")
                },
                List<Token> {
                    Token(TokenName::LETTER, "i")
                },
                List<Token> {
                    Token(TokenName::SOUND, "sss")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "t"),
                    Token(TokenName::LETTER, "h")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "th")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "i")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "i")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "s")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "s")
                }
            ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::LETTER, "c")
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "c")
                }
           ),
            LtsRule(
                _,
                List<Token> {
                    Token(TokenName::WORD_SEPARATOR)
                },
                _,
                List<Token> {
                    Token(TokenName::SOUND, "_")
                }
            )
        }
        << List<Token> {
            Token(TokenName::SOUND, "th"),
            Token(TokenName::SOUND, "i"),
            Token(TokenName::SOUND, "ss"),
            Token(TokenName::SOUND, "_"),
            Token(TokenName::SOUND, "sss"),
            Token(TokenName::SOUND, "i"),
            Token(TokenName::SOUND, "c")
        };
}

void PhonetizerTest::phonetize()
{
    QFETCH(List<Token>, passedLetters);
    QFETCH(List<LtsRule>, rules);
    QFETCH(List<Token>, phonetizedSounds);

    QCOMPARE(Phonetizer(rules).phonetize(passedLetters), phonetizedSounds);
}

QTEST_APPLESS_MAIN(PhonetizerTest)

#include "tst_phonetizertest.moc"
