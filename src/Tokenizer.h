#ifndef TOKENIZER
#define TOKENIZER

#include "Token.h"
#include "TokenVector.h"

struct FileData{
    const char* name;
    char* data;
    struct TokenVector vec;
    size_t length;
};


//This will create a FILE* and read its data and return a FileData struct
struct FileData CreateFileData(const char* location);
void CreateTokens(struct FileData* file);
void DeleteFileData(struct FileData* file);
#endif