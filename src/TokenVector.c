#include "TokenVector.c"

struct TokenVector CreateTokenVector(){
    struct TokenVector vec;
    vec.num_elements = 0;
    vec.element_data = malloc(0);
}

struct Token* GetToken(struct TokenVector* vec, size_t index){
    if(vec->num_elements == 0 || index >= vec->num_elements)return NULL;

    return (struct Token*)(vec->element_data + sizeof(Token) * index);
}