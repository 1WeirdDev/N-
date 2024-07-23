#ifndef AST_NODE_H
#define AST_NODE_H

#include "ASTNodeVector.h"
#include "Token.h"

enum ASTNodeTypes{
    AST_None,
    AST_L_VALUE,
    AST_R_VALUE,
    AST_CreateVariable,
    AST_CreateVariableNAssign,
    AST_AssignVar,
    AST_FunctionCreation,
    AST_FunctionCall
};

struct ASTNode{
    struct ASTNodeVector children;
    unsigned short type;
    void* left;
    void* right;
};

//Nodes only get created if the program is valid at the time of that node getting parsed
void InitializeASTNode(struct ASTNode* ast);

//Create Var Node
//Left = Create Variable Node
//Right = Value
struct ASTNode* GenLValue(const char* variable_data);
struct ASTNode* GenRValue(enum TokenType type, void* variable_data);
struct ASTNode* GenCreateVarNode(const char* variable_type, const char* variable_name);
struct ASTNode* GenCreateNAssignVarNode(const char* variable_type, const char* variable_name, struct ASTNode* value);
struct ASTNode* GenAssignVariable(const char* variable_name, struct ASTNode* variable_data);
struct ASTNode* GenFunctionCreation(const char* function_name);
struct ASTNode* GenFunctionCall(const char* function_name);

void DeleteASTNode(struct ASTNode* ast);
#endif