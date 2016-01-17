(function () {
    var TokenName = {
        WORD: 'WORD',   // This token name must match the C++ TokenName::WORD value (structures/token.h)
        NUMBER: 'NUMBER',
        PUNCTUATION: 'PUNCTUATION'
    };

    function createToken(name, data) {
        return {
            name: name,
            data: data
        };
    }
    
    var isNumber = function (token) {
        return token.name === TokenName.NUMBER;
    }

    var isPunctuation = function (symbol) {
        return function (token) {
            return token.name === TokenName.SPACING && token.data === symbol;
        };
    }

    return [
        {
            matchingSequence: [isNumber],
            handler: function (tokens) {
                var CHARACTER_NAMES = {
                    '0': 'ноль',
                    '1': 'один',
                    '2': 'два',
                    '3': 'три',
                    '4': 'четыре',
                    '5': 'пять',
                    '6': 'шесть',
                    '7': 'семь',
                    '8': 'восемь',
                    '9': 'девять',
                    ',': 'запятая'
                }
                var token = tokens[0];
                return token.data.split('').map(function (digit) {
                    return createToken(TokenName.WORD, CHARACTER_NAMES[digit]);
                });
            }
        },
        {
            matchingSequence: [isPunctuation('.')],
            handler: function (_) {
                return [ createToken(TokenName.WORD, 'точка') ];
            }
        },
        {
            matchingSequence: [isPunctuation(',')],
            handler: function (_) {
                return [ createToken(TokenName.WORD, 'запятая') ];
            }
        },
        {
            // Lowercase all words
            matchingSequence: [
                // Match any token
                function (_) { return true; }
            ],
            handler: function (tokens) {
                var token = tokens[0];
                return [ createToken(token.name, token.data.toLowerCase()) ];
            }
        }
    ];
})();