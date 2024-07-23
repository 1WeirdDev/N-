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
        DeleteASTNode(ast->right);
        free(ast->left);
        break;
    case AST_AssignVar:
        free(ast->right);
        break;
    case AST_FunctionCreation:
        for(size_t i = 0; i < ast->children.num_elements; i++)
            DeleteASTNode(ast->children.element_data[i]);
        ASTNodeVectorDeleteData(&ast->children);
        break;
    case AST_FunctionCall:
        for(size_t i = 0; i < ast->children.num_elements; i++)
            DeleteASTNode(ast->children.element_data[i]);
        ASTNodeVectorDeleteData(&ast->children);
        break;
    
    }
    ASTNodeVectorDeleteData(&ast->children);
    free(ast);
}

struct ASTNode* GenLValue(const char* variable_name){
    struct ASTNode* node = malloc(sizeof(struct ASTNode));
    InitializeASTNode(node);
    node->type = AST_L_VALUE;

    //L Values are always identifiers for now atleast
    node->left = (void*)variable_name;
    //node->right = (void*)variable_data;
    return node;
}
struct ASTNode* GenRValue(enum TokenType type, void* variable_data){
    struct ASTNode* node = malloc(sizeof(struct ASTNode));
    InitializeASTNode(node);
    node->type = AST_R_VALUE;

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

struct ASTNode* GenAssignVariable(const char* variable_name, struct ASTNode* variable_data){
    //Varaible Data will be a token*
    //Right is a l or rvalue
    struct ASTNode* node = malloc(sizeof(struct ASTNode));
    InitializeASTNode(node);
    node->type = AST_AssignVar;
    node->left = (void*)variable_name;
    node->right =variable_data;
    return node;
}

//The children will be ASTNode*
//Left is function identifier
//Right is the function arg list
struct ASTNode* GenFunctionCreation(const char* function_name, struct ASTNodeVector vector){
    struct ASTNode* node = malloc(sizeof(struct ASTNode));
    node->children.num_elements = vector.num_elements;
    node->children.element_data = vector.element_data;
    node->type = AST_FunctionCreation;
    node->left = (void*)function_name;
    node->right = NULL;
    return node;
}
//The children will the funciton arguments
//Argument ASTNode* is RValue
struct ASTNode* GenFunctionCall(const char* function_name, struct ASTNodeVector vector){
    struct ASTNode* node = malloc(sizeof(struct ASTNode));
    node->children.num_elements = vector.num_elements;
    node->children.element_data = vector.element_data;
    node->type = AST_FunctionCall;
    node->left = (void*)function_name;
    return node;
}