#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys\stat.h>
#include <ctype.h>

#define MAX_FILENAME 30
#define MAX_COMAND_LENGTH 10
#define NUMBER_OF_COMANDS 12

enum comands {  hlt  = 0, 
                in   = 1, out, 
                push = 3,
                add  = 4, sub, mult, segm,
                pow  = 8, sqrt,
                sin  = 10, cos};

struct text;

void Compiler (struct text* file);

size_t Size_Getter  (const char* name);
size_t Text_Reader  (text* program);

void NameProcessing (text* program);

void CodeMaker      (text* program);
void ComandsConverter (char* word, FILE* bite_code, int count_lines);

struct text
{
    size_t size;
    char* name;
    char* ptr;
};
