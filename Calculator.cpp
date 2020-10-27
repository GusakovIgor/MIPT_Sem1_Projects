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
    processor->calls = StackConstruct (NUM_COMANDS);
    processor->registers = (double*) calloc (NUM_REGISTERS + 1, sizeof(double));
    
    ///Signature
    printf ("%c%cver", processor->code[0], processor->code[1]);
    printf ("%d\n", *(int*)(processor->code + 4));              // Plus 4 cause memory distributor expands const char[3] to const char[4]
    processor->pc = sizeof(FileHeader);                         // And only after that version is
    //------------------------
    
    #define DEF_CMD(name, num, arg, code)   \
            case CMD_##name: code break;    \
            
    while (processor->pc < code_size)
    {
        //printf("OK");
        //printf("%d\n", (int) processor->code[processor->pc]);
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
    processor->calls = StackDestruct (processor->calls);
    
    return 0;
}
