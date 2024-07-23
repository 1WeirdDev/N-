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

struct Token GetToken(unsigned int add){
    if(current_token_index + add < file_data->vec.num_elements)return file_data->vec.element_data[current_token_index + add];
    struct Token t;
    t.type = TTEOF;
    t.value = NULL;
    return t;
}

//Parses a R or L value
struct ASTNode* ParseValue(){
    struct Token token = GetToken(0);
    struct ASTNode* node = NULL;

    switch(token.type){
    case TTInt32:
        node = GenRValue(token.type, token.value);
        break;
    default:
        PrintParserError(PARSE_ERROR_INVALID_EXPRESSION, token.line, "Invalid Expression");
    }

    current_token_index++;
    return node;
}
struct ASTNode* ParseAST(){
    struct Token token = GetToken(0);

    switch(token.type){
    case TTKeyword:
        if(IsKeywordValueType(token.value)){
            printf("Keyword is value type\n");

            struct Token identifier_token = GetToken(1);

            if(identifier_token.type != TTIdentifier){
                PrintParserError(PARSE_ERROR_INVALID_VARIABLE_DECLERATION, identifier_token.line, "Invalid Identifier creation.");
                return;
            }

            //Check for variable decleration and or assignment
            struct Token next = GetToken(2);
            if(next.value == ';'){
                //Ok simple variable decleration
                return GenCreateVarNode(token.value, identifier_token.value);
            }
            if(next.value == '='){
                //Variable creation and assignment
                current_token_index+=2;
                struct Token expression_token = GetToken(0);
                struct ASTNode* right = ParseValue();

                if(right == NULL){
                    PrintParserError(PARSE_ERROR_INVALID_EXPRESSION, expression_token.line, "Invalid expression for variable assignment");
                    return;
                }

                //TODO: free mem
                if(GetToken(0).value != ';'){
                    PrintParserError(PARSE_ERROR_NO_SEMICOLON, identifier_token.line, "Expected semicolon");
                    return;
                }

                return GenCreateNAssignVarNode(token.value, identifier_token.value, right);
            }

            PrintParserError(PARSE_ERROR_INVALID_VARIABLE_DECLERATION, identifier_token.line, "Invalid Identifier creation. Expected decleration or assignment");
            return;
        }
        break;
    default:
        printf("INVALID\n");
        exit(-1);
        return NULL;
    }
}
void ParseFileData(struct FileData* new_file_data){
    file_data = new_file_data;
    p_current_file = file_data;
    current_token_index = 0;
    
    struct ASTNodeVector vector;
    vector.element_data = NULL;
    vector.num_elements = 0;

    for(current_token_index = 0; current_token_index < new_file_data->vec.num_elements; current_token_index++){
        ASTNodeVectorPushBack(&vector, ParseAST());
    }

    printf("AST Nodes count %d\n", vector.num_elements);
    for(size_t i = 0; i < vector.num_elements; i++){
        printf("I %d is type %d\n", i, vector.element_data[i]->type);
        DeleteASTNode(vector.element_data[i]);
    }

    ASTNodeVectorDeleteData(&vector);
}