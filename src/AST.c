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
        case AST_AssignVar:
        free(ast->right);
        break;
    }
    ASTNodeVectorDeleteData(&ast->children);
    free(ast);
}

struct ASTNode* GenRValue(enum TokenType type, void* variable_data){
    struct ASTNode* node = malloc(sizeof(struct ASTNode));
    InitializeASTNode(node);
    node->type = AST_CreateVariable;

    node->left = (void*)type;
    node->right = (void*)variable_data;
    return node;
}
struct ASTNode* GenCreateVarNode(const char* variable_type, const char* variable_name){
    struct ASTNode* node = malloc(sizeof(struct ASTNode));
    InitializeASTNode(node);
    node->type = AST_CreateVariable;

    node->left = (void*)variable_type;
    node->right = (void*)variable_name;
    return node;
}
struct ASTNode* GenCreateNAssignVarNode(const char* variable_type, const char* variable_name, struct ASTNode* value){
    //LEFT = (ASTNode*) GenCreateVariableNode
    //Right = RLValue ASTNode*
    struct ASTNode* node = malloc(sizeof(struct ASTNode));
    InitializeASTNode(node);
    node->type = AST_CreateVariableNAssign;
    node->left = (void*)(GenCreateVarNode(variable_type, variable_name));
    node->right = (void*)value;
    return node;
}

struct ASTNode* GenAssignVariable(const char* variable_name, enum TokenType variable_type, void* variable_data){
    //Varaible Data will be a token*
    struct ASTNode* node = malloc(sizeof(struct ASTNode));
    InitializeASTNode(node);
    node->type = AST_AssignVar;
    node->left = (void*)variable_name;
    node->right = GenRValue(variable_type, variable_data);
    return node;
}