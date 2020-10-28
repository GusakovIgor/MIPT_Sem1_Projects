//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <assert.h>
#include <sys\stat.h>
#include <memory.h>
#include <ctype.h>
#include "Stack/MyStack.h"

const int MAX_FILENAME   = 30;      // For program->name
const int MAX_LABLE_NAME = 15;
const int MAX_CODE_LEN   = 100000;  // For program->bin_buff
const int MAX_COMAND_LEN = 10;       
const int MAX_NUM_LABLES = 500;
const int NUM_ASM        = 2;       // For number of calling Assembler()
const int NUM_COMANDS    = 26;      // For arrays with names of comands
const int NUM_REGISTERS  = 6;       // For arrays with names of registers
const int VERSION        = 5;

const int NUMBER = 0;
const int WORD   = 1;

// DEFINING COMMANDS
#define DEF_CMD(name, num, arg, code)  \
                CMD_##name = num,

enum comands
{
    #include "Commands.h"
};

#undef DEF_CMD

// END OF DEFINING COMMANDS

enum registers {    RAX = 1, 
                    RBX = 2, 
                    RCX = 3, 
                    RDX = 4,
                    CAT = 5,
                    MYAU = 6     };
            

struct text;
struct CPU;
struct FileHeader;
struct lable;


// Text_processing file
text* ProgramConstructor (struct text* program);

size_t TextGetter   (text* program);
size_t SizeGetter   (const char* name);
size_t TextReader   (text* program, size_t size);

size_t WordsCounter (char* text, size_t num_symbals);
char*  NameProcessing (char* name);
//---------------------------------------------------


// Assembler file
void Assembler (text* program, char* bin_buff, lable* lables, int ofs);
void Sign_maker (char* bin_buff, int* ofs);
void MakeLable      (lable* lables, char* temp, char* check, int* ofs, int count);
int  SearchLable    (lable* lables, char* temp);

void ComplicComProcessing (char* bin_buff, int* ofs, char* temp, int count, lable* lables, int num);
void PushProcessing (char* bin_buff, int* ofs, char* temp, int count);                  // Thanks Uliana for that functions
void PopProcessing  (char* bin_buff, int* ofs, char* temp, int count);                  // (I was trying to make it all in DEF_CMD macro)
void JmpProcessing  (char* bin_buff, int* ofs, char* temp, int count, lable* lables);   //
int  FindRegNumber  (char* temp, int count);
//--------------------------------------------------

struct text
{
    size_t num_words;
    char* name;
    char* buff;
};


struct CPU
{
    MyStack* stack;
    MyStack* calls;
    
    int pc;
    
    double* registers;
    
    char* code;
};


struct FileHeader
{
    const char signature[3];
    int version;
};


struct lable
{
    int adr;
    char name[MAX_LABLE_NAME];
};
