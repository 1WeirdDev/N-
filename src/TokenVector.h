#ifndef TOKENVECTOR
#define TOKENVECTOR
#include "Token.h"
#include <stdlib.h>
#include <stddef.h>

struct TokenVector{
    size_t num_elements;
    struct Token* element_data;
};

void TokenVectorPushBack(struct TokenVector* vec, struct Token token);
void TokenVectorDelete(struct TokenVector* vec);
#endif