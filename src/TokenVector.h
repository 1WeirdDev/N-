#ifndef TOKENVECTOR
#define TOKENVECTOR
#include "Token.h"
#include <stdlib.h>

struct TokenVector{
    size_t num_elements;
    void* element_data;
};

struct TokenVector CreateTokenVector();
struct Token GetToken();
#endif