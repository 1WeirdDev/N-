#ifndef AST_NODE_H
#define AST_NODE_H

#include "ASTNodeVector.h"

enum ASTNodeTypes{
    AST_None,
    AST_CreateVariable,
    AST_CreateVariableNAssign,
};

struct BareNode{
    unsigned short id;
    void* data;
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
struct ASTNode* GenCreateVarNode(const char* variable_type, const char* variable_name);
struct ASTNode* GenCreateNAssignVarNode(const char* variable_type, const char* variable_name, void* value);


void DeleteASTNode(struct ASTNode* ast);
#endif