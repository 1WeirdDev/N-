#include "AST.h"

void InitializeASTNode(struct ASTNode* ast){
    ast->children.element_data = NULL;
    ast->children.num_elements = 0;
    ast->left = NULL;
    ast->right = NULL;
}
void DeleteASTNode(struct ASTNode* ast){
    //Some nodes will have different values for left and right
    //Depending on the node we will have to delete or keep different data
    switch(ast->type){
        case AST_CreateVariableNAssign:
        free(ast->left);
        break;
    }
    ASTNodeVectorDelete(&ast->children);
    free(ast);
}

struct ASTNode* GenCreateVarNode(const char* variable_type, const char* variable_name){
    struct ASTNode* node = malloc(sizeof(struct ASTNode));
    InitializeASTNode(node);
    node->type = AST_CreateVariable;

    node->left = (void*)variable_type;
    node->right = (void*)variable_name;
    return node;
}
struct ASTNode* GenCreateNAssignVarNode(const char* variable_type, const char* variable_name, void* value){
    //LEFT = (ASTNode*) GenCreateVariableNode
    //Right = (void*) any type of data
    struct ASTNode* node = malloc(sizeof(struct ASTNode));
    InitializeASTNode(node);
    node->type = AST_CreateVariableNAssign;
    node->left = (void*)(GenCreateVarNode(variable_type, variable_name));
    node->right = (void*)5;
    return node;
}