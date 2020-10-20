#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys\stat.h>
#include <ctype.h>

#define MAX_FILENAME 30
#define MAX_COMAND_LENGTH 10
#define NUM_COMANDS 13
#define NUM_REGISTERS 4

enum comands   {    HLT  = 0, 
                    IN   = 1, OUT, 
                    PUSH = 3, POP,
                    ADD  = 5, SUB, MULT, SEGM,
                    POW  = 9, SQRT,
                    SIN  = 11, COS  };

enum registers {    RAX = 0, 
                    RBX = 1, 
                    RCX = 2, 
                    RDX = 3     };
                    
#define CheckErrors(cur_comand, cur_register, last_comand, line)                                                            \
            if (last_comand != PUSH && last_comand != POP && cur_register != NUM_REGISTERS + 1)                             \
            {                                                                                                               \
                printf ("\n\nINPUT ERROR: You're trying to get register without comands for it on line %d\n", line);        \
                printf ("             Please, use push (if you want to put register in stack)\n");                          \
                printf ("                     or  pop  (if you want to get top from stack and put it in register)\n\n");    \
                assert (!"OK");                                                                                             \
            }                                                                                                               \
                                                                                                                            \
            if ((last_comand == PUSH || last_comand == POP) && cur_register > NUM_REGISTERS && (cur_comand >= NUM_COMANDS)) \
            {                                                                                                               \
                printf ("\n\nINPUT ERROR: You're trying to use wrong register on line %d\n", line);                         \
                printf ("There are registers, you can use:\n");                                                             \
                for (int count = 1; count <= NUM_REGISTERS; count++)                                                        \
                    printf ("%s\n", register_array[count]);                                                                 \
                printf ("\n");                                                                                              \
                assert (!"OK");                                                                                             \
            }                                                                                                               \
                                                                                                                            \
            if ((cur_comand < 0 || cur_comand >= NUM_COMANDS) && last_comand != PUSH && cur_register == NUM_REGISTERS + 1)  \
            {                                                                                                               \
                printf ("\n\nINPUT ERROR: There is incorrect comand or register name in input file on line %d\n", line);    \
                printf ("Comand   code - %d (NUM_COMANDS = %d)\n", t, NUM_COMANDS);                                         \
                printf ("Register code - %d  (NUM_REGISTERS = %d)\n\n", i, NUM_REGISTERS);                                  \
                assert (!"OK");                                                                                             \
            }                                                                                                               \
                                                                                                                            \
            

struct text;

void Compiler (text* program);

size_t TextGetter   (text* program);
size_t SizeGetter   (const char* name);
size_t TextReader   (text* program, size_t size);
size_t LinesCounter (char* text, size_t num_symbals);

void NameProcessing (text* program);

void CodeMaker        (text* program, size_t size);
int  ComandsConverter (char* word, FILE* bite_code);

struct text
{
    size_t num_lines;
    char* name;
    char* ptr;
};
