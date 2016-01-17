(function () {
    var TokenName = {
        WORD: 'WORD',   // This token name must match the C++ TokenName::WORD value (structures/token.h)
        NUMBER: 'NUMBER',
        SPACING: 'SPACING',
        PUNCTUATION: 'PUNCTUATION'
    };

    function createToken(name, data) {
        return {
            name: name,
            data: data
        };
    }

    var isWord = function (token) {
        return token.name === TokenName.WORD;
    }

    var isNumber = function (token) {
        return token.name === TokenName.NUMBER;
    }

    var isSpacing = function (token) {
        return token.name === TokenName.SPACING;
    }

    var isPunctuation = function (symbol) {
        return function (token) {
            return token.name === TokenName.PUNCTUATION && token.data === symbol;
        };
    }

    return [
        // «красно-синий» → «красно синий»
        {
            matchingSequence: [isWord, isPunctuation('-'), isWord],
            handler: function (tokens) {
                return [ tokens[0], tokens[2] ];
            }
        },
        // «25 789» → «25789»
        {
            matchingSequence: [isNumber, isSpacing, isNumber],
            handler: function (tokens) {
                return [ createToken(TokenName.NUMBER, tokens[0].data + tokens[2].data) ];
            }
        },
        // A fractional number («45» + «.» + «789») → a single token
        {
            matchingSequence: [
                isNumber,
                function(token) { return isPunctuation('.')(token) || isPunctuation(',')(token) },
                isNumber
            ],
            handler: function (tokens) {
                return [ createToken(TokenName.NUMBER, tokens[0].data + ',' + tokens[2].data) ];
            }
        },
    ];
})();