#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "Lexer.h"
#include "Parser.h"
#include "ErrorCodes.h"

int main(int argc, char** argv){
    bool is_verbose = false;
    for(unsigned char i = 1; i < argc; i++){
        const char* arg = argv[i];

        if(strcmp(arg, "-i") == 0){
            is_verbose = true;
        }else{
            printf("Unidentified Argument (%s) %d\n", arg, strcmp(arg, "-i"));
            return NPP_UNIDENTIFIED_ARG;
        }
    }

    puts("LEXING\n\n\n");
    const char* location = "res/Test.n";
    printf("Reading file %s\n", location);
    
    struct FileData t = CreateFileData(location);
    CreateTokens(&t);
    puts("Parsing\n\n\n");

    //TODO: Do stuff with toknes
    ParseFileData(&t);

    DeleteFileData(&t);
}