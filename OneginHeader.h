#include <sys\stat.h>
#include <stdlib.h>
#include <locale.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

#define ERROR -1
#define OK 0

void Unit_Tests (int argc, const char* argv[]);
void Help_Print (int error);
void Size_Getter_Test ();
void Lines_Counter_Test ();

void Text_Getter (const char* name, struct arr_info* text);

size_t Size_Getter (const char* name);
size_t Text_Reader (char* text, size_t size, const char* name);
size_t Lines_Counter (char* text, size_t num_elements);

struct str_info* Struct_Array_Maker (char* text, size_t num_symbols, size_t num_lines);
size_t ZeroSkipper (char* text, size_t i);

int MyQuickSort (struct str_info* sarray, int left, int right, int (*Comp)(const void*, const void*) );
int Forward_Strcmp_rus  (const void* pointer_str1, const void* pointer_str2);
int Backward_Strcmp_rus (const void* pointer_str1, const void* pointer_str2);
void Swap (struct str_info* str1, struct str_info* str2);
int isspace (char c);

void Memory_Cleaner (int num_arg, ...);
void FileNuller (const char* file_name);
void Text_Writer (const char* DistFileName, size_t num_lines, struct str_info* struct_array, int mode);

struct str_info 
{
    char* ptr;
    char* end_ptr;
    size_t size;
};
    
struct arr_info
{
    char*  arr_ptr;
    size_t num_sym;
};
