#include "Token.h"
#include <string.h>
#include <stdio.h>

const char* keywords[KEYWORDS] = {"int", "void"};
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
    case TTSpecial:
        return "Special";
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
const char* int_name = "int";
bool IsKeywordValueType(const char* value){
    if(strcmp(value, int_name) == 0){
        return true;
    }
    return false;
}