#include <stdbool.h>
#include <stdio.h>

#include "Tokenizer.h"
#include "Token.h"
#include "ErrorCodes.h"

int main(int argc, char** argv){
    bool is_verbose = false;
    for(unsigned char i = 1; i < argc; i++){
        const char* arg = argv[i];

        if(arg == "-i"){
            is_verbose = true;
        }else{
            printf("Unidentified Argument %s\n", arg);
            return NPP_UNIDENTIFIED_ARG;
        }
    }
    const char* location = "res/Test.n";
    printf("Reading file %s\n", location);
    
    struct FileData t = CreateFileData(location);
    CreateTokens(&t);
}