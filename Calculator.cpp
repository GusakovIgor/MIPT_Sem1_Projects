#include "CPU_header.h"

int main (int argc, const char* argv[])
{
    CPU* processor = (CPU*) calloc (1, sizeof(CPU));


    char* file_name = (char*) calloc (MAX_FILENAME, sizeof(char));
    file_name = (char*) argv[1];
    file_name = NameProcessing (file_name);
    
    FILE* file = fopen (file_name, "rb");
    
    processor->code = (char*) calloc (MAX_CODE_LEN, sizeof(char));                  // HOW CAN WE SAVE THIS MEMORY?
    size_t code_size = fread (processor->code, sizeof(char), MAX_CODE_LEN, file);
    
    fclose (file);
    
    free (file_name);
    
    processor->stack = StackConstruct (NUM_COMANDS);
    processor->registers = (double*) calloc (NUM_REGISTERS + 1, sizeof(double));
    processor->pc = 0;
    
    
    #define DEF_CMD(name, num, arg, code)   \
            case CMD_##name: code break;    \
            
    while (processor->pc < code_size)
    {
        switch ( (int) processor->code[processor->pc])
        {
            #include "commands.h"
        }
        
        /*DEBUG PRINTFS OF STACK
        printf("processor->pc = %d\n", processor->pc);
        for (int i = 0; i < processor->stack->capacity; i++)
        {                                                                                                                 
            printf("    ");                                                                                      
            if (isnan((processor->stack->array)[i]))                                                                                      
                printf("* ");                                                                                                            
            else                                                                                                         
                printf("  "); 
                                                                                                              
            printf("[%d] = %lg\n", i, (processor->stack->array)[i]);                                                            
        }
        printf("\n");
        
        for (int i = 1; i < NUM_REGISTERS + 1; i++)
        {
            printf ("%lg  ", processor->registers[i]);
        }
        printf("\n\n");
        */
        
    }
    #undef DEF_CMD
    
    processor->stack = StackDestruct (processor->stack);
    
    return 0;
}
