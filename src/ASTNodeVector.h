#ifndef AST_NODE_VECTOR_H
#define AST_NODE_VECTOR_H

#include <stdlib.h>
#include <stddef.h>

struct ASTNode;
struct ASTNodeVector{
    size_t num_elements;
    struct ASTNode* element_data;
};

void ASTNodeVectorPushBack(struct ASTNodeVector* vec, struct ASTNode ast);
void ASTNodeVectorDelete(struct ASTNodeVector* vec);

#endif