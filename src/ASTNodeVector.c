#include "ASTNodeVector.h"
#include "AST.h"
#include <string.h>
#include <stdio.h>

void ASTNodeVectorPushBack(struct ASTNodeVector* vec, struct ASTNode ast){
    struct ASTNode* new_data = malloc((vec->num_elements + 1) * sizeof(struct ASTNode));
    memcpy(new_data, vec->element_data, vec->num_elements * sizeof(struct ASTNode));
    new_data[vec->num_elements] = ast;
    free(vec->element_data);
    vec->element_data = new_data;
    vec->num_elements++;
}

void ASTNodeVectorDelete(struct ASTNodeVector* vec){
    if(vec->element_data != NULL){
        free(vec->element_data);
        vec->element_data = NULL;
        vec->num_elements = 0;
    }
}