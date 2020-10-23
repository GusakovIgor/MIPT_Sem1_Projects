#include "CPU_header.h"

int main (int argc, const char* argv[])
{
    text* program = (text*) calloc (1, sizeof(text*));
    
    program->name = (char*) calloc (MAX_FILENAME, sizeof(char));
    program->name = (char*) argv[1];
    assert (strlen(program->name) < MAX_FILENAME);
    
    program = ProgramConstructor (program);
    
    Assembler (program);
    
    free (program->name);
    free (program->buff);
    free (program->bin_buff);
    free (program);
    
    return 0;
}


void Assembler (text* program)
{
    program->bin_buff = (char*) calloc (MAX_CODE_LEN, sizeof(char));
    char* temp = (char*) calloc (MAX_COMAND_LEN, sizeof(char));
    
    int ofs = 0;
    int pos = 0;
    int com_len = 0;
    int reg_number = 0;
    
    for (int count = 0; count < program->num_words; count++)
    {
        sscanf (program->buff + pos, "%s%n", temp, &com_len);
        pos += com_len;
        
        if (strcmp(temp, "hlt") == 0)
        {
            program->bin_buff[ofs] = CMD_HLT;
            ofs += sizeof(char);
        }
        else if (strcmp(temp, "in")  == 0)
        {
            program->bin_buff[ofs] = CMD_IN;
            ofs += sizeof(char);
        }
        else if (strcmp(temp, "out") == 0)
        {
            program->bin_buff[ofs] = CMD_OUT;
            ofs += sizeof(char);
        }
        else if (strcmp(temp, "push") == 0)
        {
            program->bin_buff[ofs] = CMD_PUSH;
            ofs += sizeof(char);
            count++;
            
            sscanf (program->buff + pos, "%s%n", temp, &com_len);
            pos += com_len;
            
            if (strpbrk(temp, "123456789"))
            {
                program->bin_buff[ofs] = 1;
                ofs += sizeof(char);
                
                *((double*) (program->bin_buff + ofs)) = atof(temp);
                ofs += sizeof(double);
            }
            else
            {
                program->bin_buff[ofs] = 2;
                ofs += sizeof(char);
                
                reg_number = FindRegNumber (temp, count);
                program->bin_buff[ofs] = reg_number;
                ofs += sizeof(char);
            }
        }
        else if (strcmp(temp, "pop") == 0)
        {
            program->bin_buff[ofs] = CMD_POP;
            ofs += sizeof(char);
            count++;
            
            sscanf (program->buff + pos, "%s%n", temp, &com_len);
            pos += com_len;
            
            reg_number = FindRegNumber (temp, count);
            program->bin_buff[ofs] = reg_number;
            ofs += sizeof(char);
        }
        else if (strcmp(temp, "add") == 0)
        {
            program->bin_buff[ofs] = CMD_ADD;
            ofs += sizeof(char);
        }
        else if (strcmp(temp, "sub") == 0)
        {
            program->bin_buff[ofs] = CMD_SUB;
            ofs += sizeof(char);
        }
        else if (strcmp(temp, "mult") == 0)
        {
            program->bin_buff[ofs] = CMD_MULT;
            ofs += sizeof(char);
        }
        else if (strcmp(temp, "segm") == 0)
        {
            program->bin_buff[ofs] = CMD_SEGM;
            ofs += sizeof(char);
        }
        else if (strcmp(temp, "pow") == 0)
        {
            program->bin_buff[ofs] = CMD_POW;
            ofs += sizeof(char);
        }
        else if (strcmp(temp, "sqrt") == 0)
        {
            program->bin_buff[ofs] = CMD_SQRT;
            ofs += sizeof(char);
        }
        else if (strcmp(temp, "sin") == 0)
        {
            program->bin_buff[ofs] = CMD_SIN;
            ofs += sizeof(char);
        }
        else if (strcmp(temp, "cos") == 0)
        {
            program->bin_buff[ofs] = CMD_COS;
            ofs += sizeof(char);
        }
    }
    
    FILE* code = fopen(program->name, "w+b");   // Why not creating file when mode is "wb"?...
    fwrite (program->bin_buff, ofs, 1, code);
    fclose(code);
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
