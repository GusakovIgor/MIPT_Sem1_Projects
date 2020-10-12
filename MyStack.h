#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum errors  { HASH_PROTECTION_ERROR = -7, CANARY_STACK_PROTECTION_ERROR, CANARY_ARRAY_PROTECTION_ERROR, 
                                      CONSTRUCTION_ERROR, MEMORY_ERROR, SIZE_ERROR, CHECK_IF_EMPTY_ERROR, FILLING_ERROR, I_AM_OK };
                                             
typedef unsigned long long can_type;
#define CANARY_SIZE sizeof(can_type)

#define CANARY_1L ((can_type*)((char*)stk->array - CANARY_SIZE))
#define CANARY_1R ((can_type*)((char*)stk->array + stk->capacity*sizeof(double)))

#define CANARY_2L ((can_type*)((char*)stk - CANARY_SIZE))
#define CANARY_2R ((can_type*)((char*)stk + sizeof(MyStack)))

#define MIN_CAPACITY 1

#define StackDump(stk, func)                                                                                              \
        FILE* Logs = fopen("Logs.txt", "a");                                                                              \
        int er_code = StackOK (stk);                                                                                      \
        if (er_code != 1)                                                                                                 \
        {                                                                                                                 \
          fprintf(Logs, "ERROR in function: %s\n", #func);                                                                \
          fprintf(Logs, "ERROR with code: %d\n", er_code);                                                                \
          switch (er_code)                                                                                                \
          {                                                                                                               \
            case (-7):  fprintf(Logs, "HASH_PROTECTION_ERROR: Array has changed, but it shouldn't\n");  \
                        break;                                                                                            \
            case (-6):  fprintf(Logs, "CANARY_STACK_PROTECTION_ERROR: Elements outside stack structure are changing\n");  \
                        break;                                                                                            \
            case (-5):  fprintf(Logs, "CANARY_ARRAY_PROTECTION_ERROR: Elements outside array are changing\n");            \
                        break;                                                                                            \
            case (-4):  fprintf(Logs, "CONSTRUCTION_ERROR: Pointer on stk is NULL\n");                                    \
                        break;                                                                                            \
            case (-3):  fprintf(Logs, "MEMORY_ERROR: Pointer on array is NULL\n");                                        \
                        break;                                                                                            \
            case (-2):  fprintf(Logs, "SIZE_ERROR: Capacity < size, or size < 0. Probably expansion works incorrect\n");  \
                        break;                                                                                            \
            case (-1):  fprintf(Logs, "CHECK_IF_EMPTY_ERROR: Probably you put assert before checking if stack empty or something wrong with element stk->array[0]\n"); \
                        break;                                                                                            \
            case ( 0):  fprintf(Logs, "FILLING_ERROR: Some value in stk is NAN when it shouldn't be, or it's not NAN when should be\n"); \
                        break;                                                                                            \
          }                                                                                                               \
        }                                                                                                                 \
        fprintf(Logs, "\nTemporary Hash = %lld", stk->hsh->temp_value);                                                   \
        fprintf(Logs, "\nHash = %lld", stk->hsh->value);                                                                  \
        fprintf(Logs, "\nCanary_2 Left = %X", *CANARY_2L);                                                                    \
        fprintf(Logs, "\n%s ", #stk);                                                                                     \
        if (er_code != 1)                                                                                                 \
        {                                                                                                                 \
          fprintf(Logs, "(NOT OK) ");                                                                                     \
        }                                                                                                                 \
        else                                                                                                              \
        {                                                                                                                 \
          fprintf(Logs, "(OK) ");                                                                                         \
        }                                                                                                                 \
        fprintf(Logs, "[%p]", stk);                                                                                       \
        fprintf(Logs, "\nsize = %d\n",   (stk)->size);                                                                    \
        fprintf(Logs, "capacity = %d\n", (stk)->capacity);                                                                \
        fprintf(Logs, "\nCanary_1 Left = %X\n", *CANARY_1L );                            \
        fprintf(Logs, "array [%p]\n", stk->array);                                                                        \
        fprintf(Logs, "      {\n");                                                                                       \
        for (int i = 0; i < (stk)->capacity; i++)                                                                         \
        {                                                                                                                 \
            fprintf(Logs, "    ");                                                                                        \
            if (isnan((stk)->array[i]))                                                                                   \
            {                                                                                                             \
                fprintf(Logs, "* ");                                                                                      \
            }                                                                                                             \
            else                                                                                                          \
            {                                                                                                             \
                fprintf(Logs, "  ");                                                                                      \
            }                                                                                                             \
            fprintf(Logs, "[%d] = %lg\n", i, (stk)->array[i]);                                                            \
        }                                                                                                                 \
        fprintf(Logs, "      }\n");                                                                                       \
        fprintf(Logs, "Canary_1 Right = %X\n", *CANARY_1R );             \
        fprintf(Logs, "\nCanary_2 Right = %X", *CANARY_2R);                                                                    \
        fprintf(Logs, "\n\n\n\n\n");                                                                                      \
        fclose (Logs);                                                                                                    \
 
 
#define ASSERT_OK(stk, func)      \
        if (StackOK(stk) < 1)     \
        {                         \
          printf ("\nYou can see, what's wrong in file \"Logs.txt\"\n\n");  \
          StackDump(stk,func)    \
          assert(!"OK");          \
        }                         \
 
struct MyStack;
struct MyHash;

int StackOK (MyStack* stk);
void LogsClear ();

MyHash* Hash_Construct();
void HashMaker (MyStack* stk, const char* check_flag);
bool HashChecker (MyStack* stk);

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

struct MyHash
{
    long long int temp_value;
    long long int value;
};

struct MyStack
{
    MyHash* hsh;
    bool is_empty;
    size_t size;
    size_t capacity;
    double* array;
};
