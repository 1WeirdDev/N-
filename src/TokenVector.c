#include "TokenVector.h"
#include <string.h>
#include <stdio.h>

void TokenVectorPushBack(struct TokenVector* vec, struct Token token){
    struct Token* new_data = malloc((vec->num_elements + 1) * sizeof(struct Token));
    memcpy(new_data, vec->element_data, vec->num_elements * sizeof(struct Token));
    new_data[vec->num_elements] = token;
    free(vec->element_data);
    vec->element_data = new_data;
    vec->num_elements++;
}

void TokenVectorDelete(struct TokenVector* vec){
    if(vec->element_data != NULL){
        free(vec->element_data);
        vec->element_data = NULL;
        vec->num_elements = 0;
    }
}