#include "OneginHeader.h"

enum Sorting_Modes {FORWARD = 1, BACKWARD = 2};

int main(int argc, const char* argv[])
    {
    setlocale (LC_ALL, "Russian");
    
    if (argc > 1)
        Unit_Tests (argc, argv);
    
    
    const char*         name = "Onegin.txt";
    const char*  my_dst_name = "My_Sorted_Onegin.txt";
    const char* std_dst_name = "Std_Sorted_Onegin.txt";
    
    arr_info text = {};
    Text_Getter (name, &text);
    size_t num_lines = Lines_Counter(text.arr_ptr, text.num_sym);
    struct str_info* struct_array = Struct_Array_Maker(text.arr_ptr, text.num_sym, num_lines);
    
    FileNuller (my_dst_name);
    FileNuller (std_dst_name);
    Text_Writer(my_dst_name,  num_lines, struct_array, FORWARD);
    Text_Writer(std_dst_name, num_lines, struct_array, FORWARD);
    
    MyQuickSort(struct_array, 0, num_lines-1, Forward_Strcmp_rus);
    Text_Writer(my_dst_name, num_lines, struct_array, FORWARD);
    
    MyQuickSort(struct_array, 0, num_lines-1, Backward_Strcmp_rus);
    Text_Writer(my_dst_name, num_lines, struct_array, BACKWARD);
    
    qsort(struct_array, num_lines, sizeof(str_info), Forward_Strcmp_rus);
    Text_Writer(std_dst_name, num_lines, struct_array, FORWARD);
    
    qsort(struct_array, num_lines, sizeof(str_info), Backward_Strcmp_rus);
    Text_Writer(std_dst_name, num_lines, struct_array, BACKWARD);
    
    
    Memory_Cleaner (2, text.arr_ptr, struct_array);
    
    return 0;
    }
