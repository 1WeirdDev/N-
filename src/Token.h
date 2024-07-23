#ifndef TOKEN
#define TOKEN

#include <stdlib.h>
#include <stdbool.h>

enum TokenType{
    TTEOF   =0,
    TTIdentifier,
    TTInt32,
    TTString,
    TTKeyword,
    TTOperator,
    TTSpecial
};

struct Token{
    enum TokenType type;
    size_t line;
    size_t char_index;  //Index of token start on line
    void* value;
};

#define KEYWORDS 2
extern const char* keywords[KEYWORDS];

struct Token CreateToken(enum TokenType type, size_t line, size_t char_index, void* value);
const char* GetTokenName(struct Token token);
bool IsValueType(enum TokenType type);
bool IsKeywordValueType(const char* value);
#endif