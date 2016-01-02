(function () {
    var TokenName = {
        WORD: 'word',
        NUMBER: 'number',
        PUNCTUATION: 'punctuation'
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