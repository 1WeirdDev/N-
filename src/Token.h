#ifndef TOKEN
#define TOKEN

enum TokenType{
    TTNone = 0,
    TTint32 = 0,
};

struct Token{
    enum TokenType type;
    const char* value;
};
#endif