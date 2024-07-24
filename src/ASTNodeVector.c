#include "ASTNodeVector.h"
#include "AST.h"
#include <string.h>
#include <stdio.h>

void ASTNodeVectorPushBack(struct ASTNodeVector* vec, struct ASTNode* ast){
    struct ASTNode** new_data = malloc((vec->num_elements + 1) * sizeof(struct ASTNode*));
    memcpy(new_data, vec->element_data, vec->num_elements * sizeof(struct ASTNode*));
    new_data[vec->num_elements] = ast;
    free(vec->element_data);
    vec->element_data = new_data;
    vec->num_elements++;
}

void ASTNodeVectorDeleteData(struct ASTNodeVector* vec){
    for(size_t i = 0; i < vec->num_elements; i++){
        //DeleteASTNode(vec->element_data[i]);
    }
    free(vec->element_data);
    vec->element_data = NULL;
    vec->num_elements = 0;
}