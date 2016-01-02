(function () {
    var TokenName = {
        NUMBER: 'number',
        SPACING: 'spacing',
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
    
    var isSpacing = function (token) {
        return token.name === TokenName.SPACING;
    }
    
    return [
        {
            matchingSequence: [isNumber, isSpacing, isNumber],
            handler: function (tokens) {
                return [ createToken(TokenName.NUMBER, tokens[0].data + tokens[2].data) ];
            }
        }
    ];
})();