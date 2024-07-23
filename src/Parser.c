#include "Parser.h"
#include "ErrorCodes.h"

#include "AST.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>

struct FileData* p_current_file = NULL;

void PrintParserError(int error_code, const char* data, ...){
    va_list args;

    // Initializing argument to the
    // list pointer
    va_start(args, data);
    char* buffer = (char*)calloc(256, sizeof(char));
    vsprintf (buffer,data, args);
    printf("%s: Compiler Error while parsing. Error %d, %s\n", p_current_file->name, error_code, buffer);
    free(buffer);
    va_end(args);
}
void ParseFileData(struct FileData* file_data){
    p_current_file = file_data;
    size_t current_token_index = 0;

    struct Token GetNextToken(unsigned int add){
        if(current_token_index + add < file_data->vec.num_elements)return file_data->vec.element_data[current_token_index + add];
        struct Token t;
        t.type = TTNone;
        t.value = NULL;
        return t;
    }
    for(current_token_index = 0; current_token_index < file_data->vec.num_elements; current_token_index++){
        struct Token current_token = file_data->vec.element_data[current_token_index];
        switch(current_token.type){
        case TTKeyword:{
            if(strcmp((const char*)current_token.value, "int") == 0){
                if(GetNextToken(1).type == TTIdentifier){
                    if((char*)GetNextToken(2).value == '='){
                        //Creating a variable and assigning it
                        if(GetNextToken(3).type != TTInt32){
                            PrintParserError(PARSE_ERROR_INVALID_ASSIGNMENT, "Expected int32 for assignment, instead got %s", GetTokenName(GetNextToken(3)));
                            return;
                        }

                        struct ASTNode* ast = GenCreateNAssignVarNode((const char*)current_token.value, (const char*)GetNextToken(1).type, (void*)GetNextToken(2).value);

                        DeleteASTNode(ast);
                        current_token_index+=3;
                    }
                    continue;
                }
            }
        }break;
        default:
            PrintParserError(PARSE_ERROR_INVALID_EXPRESSION, "Invalid Expression, instead got %s",GetTokenName(current_token));
            return;
        }
    }
}