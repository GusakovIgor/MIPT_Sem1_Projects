#include "CPU_header.h"

int main (int argc, const char* argv[])
{
    CPU* processor = CPU_Construct ((char*) argv[1]);
    
    double* RAM  = RAM_Maker  ();
    double* VRAM = RAM + RAM_SIZE;
    
    SignatureCheck (processor);
    
    #define DEF_CMD(name, num, arg, code)   \
            case CMD_##name: code break;    \
            
    while (processor->pc < processor->code_size)
    {
        //printf("OK");
        //printf("%d\n", (int) processor->code[processor->pc]);
        switch ( (int) processor->code[processor->pc])
        {
            #include "commands.h"
        }
        
        //StackDebugPrint(processor)
        
    }
    #undef DEF_CMD
    
    processor = CPU_Destruct(processor);
    
    free (RAM);
    
    return 0;
}


CPU* CPU_Construct (char* file_name)
{
    CPU* processor = (CPU*) calloc (1, sizeof(CPU));
    
    CodeReader (processor, file_name);
    
    processor->stack = StackConstruct (STACK_START_SIZE);
    processor->calls = StackConstruct (STACK_START_SIZE);
    processor->registers = (double*) calloc (NUM_REGISTERS + 1, sizeof(double));
    
    return processor;
}


CPU* CPU_Destruct (CPU* processor)
{
    processor->stack = StackDestruct (processor->stack);
    processor->calls = StackDestruct (processor->calls);
    free (processor->registers);
    free (processor->code);
    
    return NULL;
}


void CodeReader (CPU* processor, char* file_name)
{
    file_name = NameProcessing (file_name);
    
    FILE* file = fopen (file_name, "rb");
    
    processor->code = (char*) calloc (MAX_CODE_LEN, sizeof(char));
    processor->code_size = fread (processor->code, sizeof(char), MAX_CODE_LEN, file);
    
    fclose (file);
}


void SignatureCheck (CPU* processor)
{
    printf ("%c%c", processor->code[0], processor->code[1]);
    printf ("%d\n", *(int*)(processor->code + 4));              // Plus 4 cause memory distributor expands const char[3] to const char[4]
    assert (*(int*)(processor->code + 4) == VERSION);           // And only after that version goes
    processor->pc = sizeof(FileHeader);
}


double* RAM_Maker ()
{
    double* RAM = (double*) calloc (RAM_SIZE + VRAM_SIZE, sizeof(double));
    for (int i = 0; i < RAM_SIZE; i++)
    {
        RAM[i] = i;
    }
    
    return RAM;
}


void StackDebugPrint (CPU* processor)
{
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
}
