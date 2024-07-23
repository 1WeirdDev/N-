#include "Token.h"

const char* keywords[KEYWORDS] = {"int"};
const char* GetTokenName(struct Token token){
    switch(token.type){
    case TTNone:
        return "NONE";
    case TTString:
        return "String";
    case TTIdentifier:
        return "Identifier";
    case TTKeyword:
        return "Keyword";
    case TTInt32:
        return "Int32";
    case TTSeperator:
        return "Seperator";
    default:
        return "Error Unidentified Token";
    }
}

struct Token CreateToken(enum TokenType type, size_t line, void* value){
    struct Token token;
    token.type = type;
    token.line = line;
    token.value = value;
    return token;
}