#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

#include "ErrorCodes.h"
#include "Tokenizer.h"

struct FileData CreateFileData(const char* location){
    FILE* file = fopen(location, "r");
    if(file == NULL){
        printf("Could not open file %s", location);
        exit(CMP_ERROR_CANT_OPEN);
    }

    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    
    char* data = (char*)malloc(size);
    if( !data ) fputs("memory alloc fails",stderr),exit(1);

    size_t read_size = fread(data, 1, size, file);
    if(read_size != size){
        printf("Reading file into buffer fails. File size %d, got %d\n", size, read_size);
        exit(CMP_ERROR_FILE_READ_FAIL);
    }
    fclose(file);

    struct FileData file_data;
    file_data.name = location;
    file_data.data = data;
    file_data.length = size;
    return file_data;
}

void CreateTokens(struct FileData* t){
    size_t line = 1;
    size_t current_character_index = 0;

    char GetNextToken(){
        if(current_character_index + 1 >= t->length)
            return 0;
        return t->data[current_character_index + 1];
    }

    void PrintLXRError(int error_code, const char* data, ...){
        va_list args;
    
        // Initializing argument to the
        // list pointer
        va_start(args, data);

        char* buffer = (char*)calloc(256, sizeof(char));
        vsprintf (buffer,data, args);
        printf("Compiler Error while lexing. Error %d, line: %d , %s\n", error_code, line, buffer);
        free(buffer);
        va_end(args);
    }
    for(current_character_index = 0; current_character_index < t->length; current_character_index++){
        char current_char = t->data[current_character_index];

        switch(current_char){
        case '\n':
            line++;
            break;
        case '/':{
            if(GetNextToken() == '/'){
                //We are in comment now
                while(true){
                    char next = GetNextToken();
                    if(next == '\n')
                        break;
                    if(next == 0){
                        printf("Compiler Error %d. Comment reached EOF\n", CMP_ERROR_EOF);
                    }
                    current_character_index++;
                }
            }
            break;
        }
        default:
            PrintLXRError(CMP_ERROR_UNIDENTIFIED_TOKEN, "Unidentified Token %c", current_char);
           // printf("CompilerError %d. Unidentified token %c\n", , current_char);
        }
    }
}
void DeleteFileData(struct FileData* file){
    free(file->data);
    file->data = NULL;
    file->length = 0;
}