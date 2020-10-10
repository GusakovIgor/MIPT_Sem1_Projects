#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum errors  { CONSTRUCTION_ERROR = -4, MEMORY_ERROR, SIZE_ERROR, CHECK_IF_EMPTY_ERROR, 
                                                      FILLING_ERROR, I_AM_OK };

#define MIN_CAPACITY 1

#define StackDump(stk)                                                                                              \
        FILE* Logs = fopen("Logs.txt", "a");                                                                        \
        int er_code = StackOK (stk);                                                                                \
        if (er_code != 1)                                                                                           \
        {                                                                                                           \
          fprintf(Logs, "ERROR with code: %d\n", er_code);                                                          \
          switch (er_code)                                                                                          \
          {                                                                                                         \
            case (-4):  fprintf(Logs, "CONSTRUCTION_ERROR: Pointer on stk is NULL\n");                              \
                        break;                                                                                      \
            case (-3):  fprintf(Logs, "MEMORY_ERROR: Pointer on array is NULL\n");                                  \
                        break;                                                                                      \
            case (-2):  fprintf(Logs, "SIZE_ERROR: Capacity < size, or size < 0. Probably expansion works incorrect\n");  \
                        break;                                                                                      \
            case (-1):  fprintf(Logs, "CHECK_IF_EMPTY_ERROR: Probably you put assert before checking if stack empty or something wrong with element stk->array[0]\n"); \
                        break;                                                                                      \
            case ( 0):  fprintf(Logs, "FILLING_ERROR: Some value in stk is NAN when it shouldn't be, or it's not NAN when should be\n"); \
                        break;                                                                                      \
          }                                                                                                         \
        }                                                                                                           \
        fprintf(Logs, "\n%s ", #stk);                                                                               \
        if (er_code != 1)                                                                                           \
        {                                                                                                           \
          fprintf(Logs, "(NOT OK) ");                                                                               \
        }                                                                                                           \
        else                                                                                                        \
        {                                                                                                           \
          fprintf(Logs, "(OK) ");                                                                                   \
        }                                                                                                           \
        fprintf(Logs, "[%p]", #stk);                                                                                \
        fprintf(Logs, "\nsize = %d\n",   (stk)->size);                                                              \
        fprintf(Logs, "capacity = %d\n", (stk)->capacity);                                                          \
        fprintf(Logs, "array [%p]\n", #stk);                                                                        \
        fprintf(Logs, "      {\n");                                                                                 \
        for (int i = 0; i < (stk)->capacity; i++)                                                                   \
        {                                                                                                           \
            fprintf(Logs, "    ");                                                                                  \
            if (isnan((stk)->array[i]))                                                                             \
            {                                                                                                       \
                fprintf(Logs, "* ");                                                                                \
            }                                                                                                       \
            else                                                                                                    \
            {                                                                                                       \
                fprintf(Logs, "  ");                                                                                \
            }                                                                                                       \
            fprintf(Logs, "[%d] = %lg\n", i, (stk)->array[i]);                                                      \
        }                                                                                                           \
        fprintf(Logs, "      }\n");                                                                                 \
        fprintf(Logs, "\n\n\n\n\n");                                                                                \
        fclose (Logs);                                                                                              \
 
 
#define ASSERT_OK(stk)      \
        if (!StackOK(stk))  \
        {                   \
          StackDump(stk)    \
          printf ("\nYou can see, what's wrong in file \"Logs.txt\"\n\n");  \
          assert(!"OK");    \
        }                   \
 
struct MyStack;

int StackOK (MyStack* stk);
void LogsClear ();

void Unit_Tests (int argc, const char* argv[]);
void Help_Print ();
void Test_StackPush ();
void Test_StackPop  ();
void Test_StackTop  ();
void Test_StackExpansion ();
void Test_StackFree ();


MyStack* StackConstruct (size_t capacity);
MyStack* StackDestruct  (MyStack* stk);

void StackExpansion (MyStack* stk);
void StackFree      (MyStack* stk);

void StackClear (MyStack* stk);
void StackPush  (MyStack* stk, double elem);
double StackPop (MyStack* stk);
double StackTop (MyStack* stk);

struct MyStack
{
    bool is_empty;
    size_t size;
    size_t capacity;
    double* array;
};
