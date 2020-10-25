#include "CPU_header.h"

int main (int argc, const char* argv[])
{
    text* program = (text*) calloc (1, sizeof(text));
    
    program->name = (char*) calloc (MAX_FILENAME, sizeof(char));
    program->name = (char*) argv[1];
    assert (strlen(program->name) < MAX_FILENAME);
    
    program = ProgramConstructor (program);
    
    Assembler (program);
    
    free (program->name);
    free (program->buff);
    
    //printf("OK");
    free (program);      //CANNOT FREE PROGRAM
    //printf("OK");
    
    return 0;
}


void Assembler (text* program)
{
    char* bin_buff = (char*) calloc (MAX_CODE_LEN, sizeof(char));
    char* temp = (char*) calloc (MAX_COMAND_LEN, sizeof(char));
    
    int ofs = 0;
    int pos = 0;
    int com_len = 0;
    int reg_number = 0;
    
    #define DEF_CMD(name, num, arg, code)                                   \
        else if (strcmp(temp, #name) == 0)                                  \
        {                                                                   \
            bin_buff[ofs] = CMD_##name;                                     \
            ofs += sizeof(char);                                            \
            printf ("%d", CMD_##name); \
                                                                            \
            if (num == CMD_push)                                            \
            {                                                               \
                count++;                                                    \
                sscanf (program->buff + pos, "%s%n", temp, &com_len);       \
                pos += com_len;                                             \
                                                                            \
                if (strpbrk(temp, "123456789"))                             \
                {                                                           \
                    bin_buff[ofs] = 1;                                      \
                    ofs += sizeof(char);                                    \
                    printf (" 1 "); \
                                                                            \
                    *((double*) (bin_buff + ofs)) = atof(temp);             \
                    ofs += sizeof(double);                                  \
                    printf ("%lg", atof(temp));   \
                }                                                           \
                else                                                        \
                {                                                           \
                    bin_buff[ofs] = 2;                                      \
                    ofs += sizeof(char);                                    \
                    printf (" 2 "); \
                                                                            \
                    reg_number = FindRegNumber (temp, count);               \
                    bin_buff[ofs] = reg_number;                             \
                    ofs += sizeof(char);                                    \
                    printf ("%d", reg_number); \
                }                                                           \
            }                                                               \
            else if (num == CMD_pop)                                        \
            {                                                               \
                count++;                                                    \
                sscanf (program->buff + pos, "%s%n", temp, &com_len);       \
                pos += com_len;                                             \
                                                                            \
                reg_number = FindRegNumber (temp, count);                   \
                bin_buff[ofs] = reg_number;                                 \
                ofs += sizeof(char);                                        \
                printf (" %d", reg_number); \
            }                                                               \
            printf("\n");   \
        }                                                                   \
    
    for (int count = 0; count < program->num_words; count++)
    {
        sscanf (program->buff + pos, "%s%n", temp, &com_len);
        pos += com_len;
        
        if (0) {}
        #include "commands.h"
        else
        {
            printf ("Syntax Error!\n");
            printf ("In word: %s\n", temp);
            assert (!"OK");
        }
    }
    #undef DEF_CMD
    
    free(temp);
    
    FILE* code = fopen(program->name, "w+b");   // Why not creating file when mode is "wb"?...
    fwrite (bin_buff, ofs, 1, code);
    fclose(code);
    
    free (bin_buff);
}

void PushProcessing (char* bin_buff, int* ofs, char* temp, int count)
{
    if (strpbrk(temp, "123456789"))
    {
        bin_buff[*ofs] = 1;
        *ofs += sizeof(char);
        printf (" 1 ");
        
        *((double*) (bin_buff + *ofs)) = atof(temp);
        *ofs += sizeof(double);
        printf ("%lg", atof(temp));
    }
    else
    {
        bin_buff[*ofs] = 2;
        *ofs += sizeof(char);
        printf (" 2 ");
        
        reg_number = FindRegNumber (temp, count);
        bin_buff[*ofs] = reg_number;
        *ofs += sizeof(char);
        printf ("%d", reg_number);
    }
}

int FindRegNumber (char* temp, int count)
{
    if (strcmp(temp, "rax") == 0)
    {
        return RAX;
    }
    else if (strcmp(temp, "rbx") == 0)
    {
        return RBX;
    }
    else if (strcmp(temp, "rcx") == 0)
    {
        return RCX;
    }
    else if (strcmp(temp, "rdx") == 0)
    {
        return RDX;
    }
    else
    {
        printf ("ERROR in word %d\n", count);
        printf ("Name of register after push or pop doesn't match any of the existing register");
        printf ("\n\n(word is any symbol-number combination that you entered)");
        assert (!"OK");
    }
}
