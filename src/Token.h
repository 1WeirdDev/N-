#ifndef TOKEN
#define TOKEN

#include <stdlib.h>

enum TokenType{
    TTNone   =0,
    TTIdentifier,
    TTInt32,
    TTString,
    TTKeyword,
    TTOperator,
    TTSeperator
};

struct Token{
    enum TokenType type;
    size_t line;
    void* value;
};

#define KEYWORDS 1
extern const char* keywords[1];

struct Token CreateToken(enum TokenType type, size_t line, void* value);
const char* GetTokenName(struct Token token);
#endif