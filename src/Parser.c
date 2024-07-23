#include "Parser.h"
#include "ErrorCodes.h"

#include "AST.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

struct FileData* p_current_file = NULL;
size_t current_token_index = 0;
struct FileData* file_data = NULL;

void PrintParserError(int error_code, int line, const char* data, ...){
    va_list args;

    // Initializing argument to the
    // list pointer
    va_start(args, data);
    char* buffer = (char*)calloc(256, sizeof(char));
    vsprintf (buffer,data, args);
    printf("%s:%d Compiler Error while parsing. Error %d, %s\n", p_current_file->name, line, error_code, buffer);
    free(buffer);
    va_end(args);
}

struct Token GetNextToken(unsigned int add){
    if(current_token_index + add < file_data->vec.num_elements)return file_data->vec.element_data[current_token_index + add];
    struct Token t;
    t.type = TTNone;
    t.value = NULL;
    return t;
}

struct ASTNode* ParseExpression(){
    
}
void ParseFileData(struct FileData* new_file_data){
    file_data = new_file_data;
    p_current_file = file_data;
    current_token_index = 0;
    
    struct ASTNodeVector vector;
    vector.element_data = NULL;
    vector.num_elements = 0;

    //TODO: free memory when invalid expression
    for(current_token_index = 0; current_token_index < file_data->vec.num_elements; current_token_index++){
        struct Token current_token = file_data->vec.element_data[current_token_index];
        switch(current_token.type){
        case TTKeyword:{
            if(strcmp((const char*)current_token.value, "int") == 0){
                if(GetNextToken(1).type == TTIdentifier){
                    if((char*)GetNextToken(2).value == '='){
                        //Creating a variable and assigning it
                        if(GetNextToken(3).type != TTInt32){
                            PrintParserError(PARSE_ERROR_INVALID_ASSIGNMENT, GetNextToken(3).line, "Expected int32 for assignment, instead got %s", GetTokenName(GetNextToken(3)));
                            return;
                        }
                        if(GetNextToken(4).value != ';'){
                            PrintParserError(PARSE_ERROR_NO_SEMICOLON, GetNextToken(4).line, "Expected ; after variable decleration");
                            return;
                        }
                        struct ASTNode* ast = GenCreateNAssignVarNode((const char*)current_token.value, (const char*)GetNextToken(1).type, (void*)GetNextToken(2).value);
                        ASTNodeVectorPushBack(&vector, ast);
                        current_token_index+=4;
                    }
                    else if((char*)GetNextToken(2).value == ';'){
                        struct ASTNode* ast = GenCreateVarNode((const char*)current_token.value, (const char*)GetNextToken(1).type);
                        ASTNodeVectorPushBack(&vector, ast);
                        current_token_index+=2;
                    }else{
                        PrintParserError(PARSE_ERROR_INVALID_EXPRESSION, GetNextToken(2).line, "Expected variable decleration or assignment, instead got %s", (const char*)GetNextToken(2).value);
                        return;
                    }
                    continue;
                }
            }
        }break;
        case TTIdentifier:{
            if((char*)GetNextToken(1).value == '='){
                if(IsValueType(GetNextToken(2).type)){
                    if(GetNextToken(3).value != ';'){
                        PrintParserError(PARSE_ERROR_NO_SEMICOLON, GetNextToken(3).line, "Expected ; after variable decleration");
                        return;
                    }
                    struct ASTNode* ast = GenAssignVariable((const char*)current_token.value, GetNextToken(2).type, (void*)GetNextToken(2).value);
                    ASTNodeVectorPushBack(&vector, ast);
                    current_token_index+=3;
                }else{
                    PrintParserError(PARSE_ERROR_INVALID_EXPRESSION, current_token.line, "Invalid Expression. Expected Value Type instead got %s",(const char*)GetNextToken(2).value);
                }
            }
        }break;
        default:
            PrintParserError(PARSE_ERROR_INVALID_EXPRESSION, current_token.line, "Invalid Expression, instead got %s",GetTokenName(current_token));
            return;
        }
    }

    printf("AST Nodes count %d\n", vector.num_elements);
    for(size_t i = 0; i < vector.num_elements; i++){
        printf("I %d is type %d\n", i, vector.element_data[i]->type);
        DeleteASTNode(vector.element_data[i]);
    }

    ASTNodeVectorDeleteData(&vector);
}