#include "SemanticAnalyzer.h"
#include <stdio.h>

void SemanticAnalyzeNodes(struct ASTNodeVector* vec){
    for(size_t i = 0; i < vec->num_elements; i++){
        struct ASTNode* node = vec->element_data[i];
        switch(node->type){
        default:

        }
    }
}