#include "Parser.h"
#include "ErrorCodes.h"

#include "AST.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

#pragma GCC diagnostic ignored "int-to-pointer-cast"

struct FileData* p_current_file = NULL;
size_t current_token_index = 0;
struct FileData* file_data = NULL;
bool has_parser_error = false;

void PrintParserError(int error_code, int line, const char* data, ...){
    va_list args;
    has_parser_error = true;
    // Initializing argument to the
    // list pointer
    va_start(args, data);
    char* buffer = (char*)calloc(256, sizeof(char));
    vsprintf (buffer,data, args);
    printf("%s:%d Compiler Error %s\n", p_current_file->name, line, buffer);
    free(buffer);
    va_end(args);
}

struct Token GetToken(int add){
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
    case TTIdentifier:
        //TODO: Check for function call
        node = GenLValue(token.value);
        break;
    default:
        PrintParserError(PARSE_ERROR_INVALID_EXPRESSION, token.line, "Invalid Expression R or L value");
    }

    current_token_index++;
    return node;
}
struct ASTNode* ParseAST(){
    struct Token token = GetToken(0);
    switch(token.type){
    case TTKeyword:
        if(IsKeywordValueType(token.value)){
            struct Token identifier_token = GetToken(1);

            if(identifier_token.type != TTIdentifier){
                PrintParserError(PARSE_ERROR_INVALID_VARIABLE_DECLERATION, identifier_token.line, "Invalid Identifier creation.");
                return NULL;
            }

            //Check for variable decleration and or assignment
            struct Token next = GetToken(2);
            if((char)next.value == ';'){
                //Ok simple variable decleration
                current_token_index+=2;
                puts("RETURNING GenCreateVarNode\n");
                return GenCreateVarNode(token.value, identifier_token.value);
            }
            if((char)next.value == '='){
                //Variable creation and assignment
                current_token_index+=3;
                struct Token expression_token = GetToken(0);
                struct ASTNode* right = ParseValue();

                if(right == NULL){
                    PrintParserError(PARSE_ERROR_INVALID_EXPRESSION, expression_token.line, "Invalid expression for variable assignment");
                    return NULL;
                }

                //TODO: free mem
                if((char)GetToken(0).value != ';'){
                    PrintParserError(PARSE_ERROR_NO_SEMICOLON, identifier_token.line, "Expected semicolon");
                    return NULL;
                }

                puts("RETURNING GenCreateNAssignVarNode\n");
                return GenCreateNAssignVarNode(token.value, identifier_token.value, right);
            }

            PrintParserError(PARSE_ERROR_INVALID_VARIABLE_DECLERATION, identifier_token.line, "Invalid Identifier creation. Expected decleration or assignment");
            return NULL;
        }
        else if(strcmp(token.value, "function")){
            struct Token function_name = GetToken(1);
            if(function_name.type != TTIdentifier){
                PrintParserError(PARSE_ERROR_EXPECTED_IDENTIFER, function_name.line, "Expected identifier for function decleration");
                return NULL;
            }
            if((char)GetToken(2).value != '('){
                PrintParserError(PARSE_ERROR_NO_PARENTHESIS, GetToken(2).line, "Expected ( after function identifier");
                return NULL;
            }
            current_token_index+=3;
            //TODO: Get Arguments
            if((char)GetToken(0).value != ')'){
                PrintParserError(PARSE_ERROR_NO_PARENTHESIS, GetToken(0).line, "Expected ) after function args");
                return NULL;
            }
            
            if((char)GetToken(1).value != '{'){
                PrintParserError(PARSE_ERROR_NO_CURLY_BRACE, GetToken(1).line, "Expected } after function arguments list");
                return NULL;
            }
            current_token_index+=2;
            
            struct ASTNodeVector vector;
            vector.num_elements = 0;
            vector.element_data = NULL;

            while(true){
                token = GetToken(0);
                if(token.type == TTEOF){
                    PrintParserError(PARSE_ERROR_EOF, GetToken(-1).line, "Expected } after function body");
                    return NULL;
                }
                if(token.value == '}')break;
                struct ASTNode* node = ParseAST();
                current_token_index++;
                
                ASTNodeVectorPushBack(&vector, node);
            }
            if((char)GetToken(0).value != '}'){
                PrintParserError(PARSE_ERROR_NO_CURLY_BRACE, GetToken(2).line, "Expected } after function arguments list");
                return NULL;
            }

            struct ASTNode* function_call = GenFunctionCreation(function_name.value);
            function_call->children.element_data = vector.element_data;
            function_call->children.num_elements = vector.num_elements;
            return function_call;
        }
        PrintParserError(PARSE_ERROR_INVALID_KEYWORD_USE, token.line, "Invalid Keyword use");
        return NULL;
        break;
    case TTIdentifier:
        struct Token next = GetToken(1);
        if((char)next.value == '='){
            current_token_index+=2;
            struct ASTNode* right = ParseValue();

            if((char)GetToken(0).value != ';'){
                PrintParserError(PARSE_ERROR_NO_SEMICOLON, GetToken(0).line, "Expected semicolon");
                return NULL;
            }
            puts("Returning variable assingment\n");
            return GenAssignVariable(token.value, right);
        }
        if((char)next.value == '('){
            //Starting function call
            //TODO: parameters
            if((char)GetToken(2).value != ')'){
                PrintParserError(PARSE_ERROR_INVALID_FUNCTION_CALL, GetToken(2).line, "Expected ) after funciton call");
                return NULL;
            }
            if((char)GetToken(3).value != ';'){
                PrintParserError(PARSE_ERROR_NO_SEMICOLON, GetToken(3).line, "Expected ; after funciton call");
                return NULL;
            }
            current_token_index+=3;
            puts("Returning function call");
            return GenFunctionCall(token.value);
        }

        PrintParserError(PARSE_ERROR_INVALID_VARIABLE_DECLERATION, GetToken(1).line, "Expected decleration or assignment");
        return NULL;
    break;
    default:
        printf("INVALID %d %c\n", token.type,token.value);
        exit(-1);
        return NULL;
    }
}
void ParseFileData(struct FileData* new_file_data){
    file_data = new_file_data;
    p_current_file = file_data;
    current_token_index = 0;
    has_parser_error = false;
    
    struct ASTNodeVector vector;
    vector.element_data = NULL;
    vector.num_elements = 0;

    for(current_token_index = 0; current_token_index < new_file_data->vec.num_elements; current_token_index++){
        struct ASTNode* ast=  ParseAST();
        ASTNodeVectorPushBack(&vector, ast);
    }

    printf("AST Nodes count %d\n", vector.num_elements);
    for(size_t i = 0; i < vector.num_elements; i++){
        printf("I %d is type %d\n", i, vector.element_data[i]->type);
        DeleteASTNode(vector.element_data[i]);
    }

    ASTNodeVectorDeleteData(&vector);

    if(has_parser_error){
        //TODO:Cleanup and exit
    }
}