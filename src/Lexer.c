#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>

#include "ErrorCodes.h"
#include "Lexer.h"

struct FileData* l_current_file = NULL;
[[nodiscard]] bool IsIdentifierStartChar(char c){
    return isalpha(c);
}
[[nodiscard]] bool IsIdentifierChar(char c){
    return(isalnum(c) || isdigit(c));
}

[[nodiscard]] bool IsNumber(char c){
    return isdigit(c);
}

struct FileData CreateFileData(const char* location){
    FILE* file = fopen(location, "rb");
    if(file == NULL){
        printf("Error %d, Cant open file %s", CMP_ERROR_CANT_OPEN, location);
        exit(CMP_ERROR_CANT_OPEN);
    }

    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    
    char* data = (char*)calloc(size, sizeof(char));
    if( !data ) fputs("memory alloc fails",stderr),exit(1);

    size_t read_size = fread(data, sizeof(char), size, file);
    fclose(file);

    struct FileData file_data;
    file_data.name = location;
    file_data.data = data;
    file_data.length = size;

    file_data.vec.element_data = NULL;
    file_data.vec.num_elements = 0;
    return file_data;
}
void PrintLXRError(int error_code, const char* data, ...){
    va_list args;
    
    // Initializing argument to the
    // list pointer
    va_start(args, data);

    char* buffer = (char*)calloc(256, sizeof(char));
    vsprintf (buffer,data, args);
    printf("%s: Compiler Error %d, %s\n", l_current_file->name, error_code, buffer);
    //printf("Compiler Error while lexing. Error %d, line: %d, %s\n", error_code, line, buffer);
    free(buffer);
    va_end(args);
}
void CreateTokens(struct FileData* file_data){
    l_current_file = file_data;
    size_t line = 1;
    size_t character_line_index = 1;    //TODO
    size_t current_character_index = 0; //Index into the file data buffer
    //char current_char = GetCurrentChar();

    char GetCurrentChar(){
        return file_data->data[current_character_index];
    }
    char GetNextChar(){
        if(current_character_index + 1 >= file_data->length)
            return 0;
        return file_data->data[current_character_index + 1];
    }

    for(current_character_index = 0; current_character_index < file_data->length; current_character_index++){
        char current_char = GetCurrentChar();
        switch(current_char){
        case ' ':
        case '\t'://TODO these special tabs might have spaces and stuff
        case '\r':
            continue;
        case '\n':
            line++;
            character_line_index = 1;
            break;
        case '/':{
            if(GetNextChar() == '/'){
                //We are in comment now
                while(true){
                    char next = GetNextChar();
                    if(next == '\n')
                        break;
                    if(next == 0)return;
                    current_character_index++;
                }

                continue;
            }
        }
        case '(':
        case ')':
        case '{':
        case '}':
        case ';':
        case ',':{
            TokenVectorPushBack(&file_data->vec, CreateToken(TTSpecial, line, character_line_index, (void*)current_char));
            break;
        }
        case '-':
        case '+':
        case '*':
        case '=':{
            struct Token t = CreateToken(TTOperator, line, character_line_index, (void*)current_char);
            TokenVectorPushBack(&file_data->vec, t);
            break;
        }
        default:{
            if(IsIdentifierStartChar(current_char)){
                char* identifier = calloc(2, sizeof(char));
                size_t iden_size = 1;
                identifier[0] = current_char;

                while(IsIdentifierChar(GetNextChar())){
                    current_character_index++;
                    iden_size++;
                    character_line_index++;

                    char* new_iden = (char*)calloc(iden_size + 1, sizeof(char));
                    current_char = GetCurrentChar();

                    strncat(new_iden,identifier, iden_size);
                    strncat(new_iden, &current_char, 1);

                    free(identifier);
                    identifier = new_iden;
                }
                struct Token t = CreateToken(TTIdentifier, line, character_line_index, (void*)identifier);

                //Check if identifier
                for(unsigned char i = 0; i < KEYWORDS; i++){
                    if(strcmp(identifier, keywords[i]) == 0){
                        t.type = TTKeyword;
                        i = KEYWORDS;
                        //Could just say break
                    }
                }
                TokenVectorPushBack(&file_data->vec, t);
                continue;
            }
            if(IsNumber(current_char)){
                char* identifier = calloc(2, sizeof(char));
                size_t iden_size = 1;
                identifier[0] = current_char;
                bool has_decimal = false;

                while(IsNumber(GetNextChar()) || GetNextChar() == '.'){
                    current_character_index++;
                    iden_size++;
                    character_line_index++;

                    char* new_iden = calloc(iden_size + 1, sizeof(char));
                    current_char = GetCurrentChar();

                    strncat(new_iden,identifier, iden_size);
                    strncat(new_iden, &current_char, 1);

                    free(identifier);
                    identifier = new_iden;

                    if(current_char == '.'){
                        if(has_decimal){
                            PrintLXRError(CMP_ERROR_UNEXPECTED_TOKEN, "Unexpected Token \x1B[31m%c\033[0m\t\t", GetCurrentChar());
                            break;
                        }

                        has_decimal = true;
                    }
                }
                
                if(has_decimal){
                    free(identifier);
                    PrintLXRError(CMP_ERROR_TESTING, "Error, doesnt support floating point numbers yet");
                    continue;
                }

                struct Token token;
                token.type = TTInt32;
                token.line = line;
                token.char_index = character_line_index;
                token.value = (void*)identifier;
                TokenVectorPushBack(&file_data->vec, token);
                continue;
            }
            PrintLXRError(CMP_ERROR_UNIDENTIFIED_TOKEN, "Character Index %d Unidentified Character: \x1B[31m%c\033[0m\t\t", (int)current_char, current_char);
           // printf("CompilerError %d. Unidentified token %c\n", , current_char);
        }
        }
        
        character_line_index++;
    }
}
void DeleteFileData(struct FileData* file){
    printf("Deleteing File Data for %s\n", file->name);
    for(size_t i = 0; i < file->vec.num_elements; i++){
        switch(file->vec.element_data[i].type){
            case 2:
                free(file->vec.element_data[i].value);
                break;
        }
    }

    TokenVectorDelete(&file->vec);

    free(file->data);
    file->data = NULL;
    file->length = 0;
}