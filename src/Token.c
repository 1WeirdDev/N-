#include "Token.h"

const char* keywords[KEYWORDS] = {"int"};
const char* GetTokenName(struct Token token){
    switch(token.type){
    case TTEOF:
        return "EOF";
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


bool IsValueType(enum TokenType type){
    switch(type){
    case TTInt32:
        return true;
    default:
        return false;
    }
}

bool IsKeywordValueType(const char* value){
    if(value == "int")return true;
    return false;
}