//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <assert.h>
#include <sys\stat.h>
#include <ctype.h>
#include "Stack/MyStack.h"

#define MAX_FILENAME 30         // For program->name
#define MAX_CODE_LEN 100000  // For program->bin_buff
#define MAX_COMAND_LEN 10    
#define NUM_COMANDS 13          // For arrays with names of comands
#define NUM_REGISTERS 4         // For arrays with names of registers


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
                    RDX = 4     };
                    
#define CheckErrors(cur_comand, cur_register, last_comand, line)                                                            \
            if (last_comand != CMD_PUSH && last_comand != CMD_POP && cur_register != NUM_REGISTERS + 1)                     \
            {                                                                                                               \
                printf ("\n\nINPUT ERROR: You're trying to get register without comands for it on line %d\n", line);        \
                printf ("             Please, use push (if you want to put register in stack)\n");                          \
                printf ("                     or  pop  (if you want to get top from stack and put it in register)\n\n");    \
                assert (!"OK");                                                                                             \
            }                                                                                                               \
                                                                                                                            \
            if ((last_comand == CMD_PUSH || last_comand == CMD_POP) && cur_register > NUM_REGISTERS && (cur_comand >= NUM_COMANDS)) \
            {                                                                                                               \
                printf ("\n\nINPUT ERROR: You're trying to use wrong register on line %d\n", line);                         \
                printf ("There are registers, you can use:\n");                                                             \
                for (int count = 1; count <= NUM_REGISTERS; count++)                                                        \
                    printf ("%s\n", register_array[count]);                                                                 \
                printf ("\n");                                                                                              \
                assert (!"OK");                                                                                             \
            }                                                                                                               \
                                                                                                                            \
            if ((cur_comand < 0 || cur_comand >= NUM_COMANDS) && last_comand != CMD_PUSH && cur_register == NUM_REGISTERS + 1)  \
            {                                                                                                               \
                printf ("\n\nINPUT ERROR: There is incorrect comand or register name in input file on line %d\n", line);    \
                printf ("Comand   code - %d (NUM_COMANDS = %d)\n", com_num, NUM_COMANDS);                                   \
                printf ("Register code - %d  (NUM_REGISTERS = %d)\n\n", reg_num, NUM_REGISTERS);                            \
                assert (!"OK");                                                                                             \
            }                                                                                                               \
                                                                                                                            \
            

struct text;
struct CPU;
struct FileHeader;


// Text_processing file
text* ProgramConstructor (struct text* program);

size_t TextGetter   (text* program);
size_t SizeGetter   (const char* name);
size_t TextReader   (text* program, size_t size);

size_t WordsCounter (char* text, size_t num_symbals);
void NameProcessing (text* program);
//---------------------------------------------------


// Assembler file
void Assembler (text* program);
int  FindRegNumber (char* temp, int count);
//--------------------------------------------------

struct text
{
    size_t num_words;
    char* name;
    char* buff;
    char* bin_buff;
};


struct CPU
{
    MyStack stack;
    
    double* code;
    
    int rip;
    
    double rax;
    double rbx;
    double rcx;
    double rdx;
};


struct FileHeader
{
    short signature;
    short version;
    
    size_t size;
};
