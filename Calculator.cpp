#include "CPU_header.h"

int main (int argc, const char* argv[])
{
    CPU* processor = (CPU*) calloc (1, sizeof(CPU*));

    // HOW TO TAKE CODE FROM BINARY FILE?
    /*
    char* file_name = (char*) calloc (MAX_FILENAME, sizeof(char));
    file_name = (char*) argv[1];
    file_name = NameProcessing (file_name);
    
    FILE* file = fopen(file_name, "rb");
    
    printf("OK\n");
    processor->code = (double*) calloc (MAX_CODE_LEN, sizeof(double));
    size_t code_size = fread(processor->code, sizeof(double), MAX_CODE_LEN, file);
    
    fclose (file);
    for (int i = 0; i < code_size; i++)
    {
        printf ("%lg\n", processor->code[i]);
    }
    
    free (processor->code);
    free (processor);
    */
    
    processor->stack = StackConstruct (capacity);
    processor->registers = (double*) calloc (NUM_REGISTERS + 1, sizeof(double));
    
    //code_size = ...
    
    while (processor->pc < code_size)
    {
        switch (code[processor->pc])
        {
            #include "commands.h"
        }
        
        processor->pc++;
    }
    
    
    return 0;
}
