#include "CPU_header.h"

int main (int argc, const char* argv[])
{
    text* program = (text*) calloc (1, sizeof(text));
    assert (program);
    
    program->name = (char*) calloc (MAX_FILENAME, sizeof(char));
    assert (program->name);
    program->name = (char*) argv[1];
    assert (strlen(program->name) < MAX_FILENAME);
    
    program = ProgramConstructor (program);
    assert (program);
    
    
    char* bin_buff = (char*) calloc (MAX_CODE_LEN,   sizeof(char));
    assert (bin_buff);
    
    lable*  lables = (lable*)  calloc (MAX_NUM_LABLES + 1, sizeof(lable));      // Plus 1 because user probably want to use labels 1-50
    assert (lables);                                                            // if MAX_NUM_LABELS = 50, so we've got labels 0-50
    
    for (int i = 0; i < MAX_NUM_LABLES + 1; i++)
    {
        lables[i].adr = -1;
    }
    
    int ofs = 1;
    for (int i = 0; i < NUM_ASM; i++)
    {
        printf ("\nIteration %d of Assembler:\n", i + 1);
        int ofs = 0;
        Sign_maker (4, bin_buff, &ofs);
        Assembler  (program, bin_buff, lables, ofs);
        printf ("\n\n\n");
    }
    /*
    for (int i = 0; i < 20; i++)
    {
        printf ("lables[%d].adr = %d,\tlables[%d].name = %s\n", i, lables[i].adr , i, lables[i].name);
    }
    */
    
    free (lables);
    free (bin_buff);
    
    free (program->name);
    free (program->buff);
    free (program);
    
    return 0;
}


void Assembler (text* program, char* bin_buff, lable* lables, int ofs)
{
    static size_t iter = 0;
    
    char* temp     = (char*) calloc (MAX_COMAND_LEN, sizeof(char)); assert (temp);
    char* check    = (char*) calloc (MAX_COMAND_LEN, sizeof(char)); assert (check);
    
    int pos = 0;
    int com_len = 0;
    int reg_number = 0;
    
    #define DEF_CMD(name, num, arg, code)                                           \
        else if (strcmp(temp, #name) == 0)                                          \
        {                                                                           \
                                                                                    \
            bin_buff[ofs] = CMD_##name;                                             \
            ofs += sizeof(char);                                                    \
            printf ("%d", CMD_##name); \
                                                                                    \
            if (arg > 0)                                                            \
            {                                                                       \
                count++;                                                            \
                sscanf (program->buff + pos, "%s%n", temp, &com_len);               \
                pos += com_len;                                                     \
                                                                                    \
                ComplicComProcessing (bin_buff, &ofs, temp, count, lables, num);    \
            }                                                                       \
                                                                                    \
            printf("\n");   \
        }                                                                           \
    
    for (int count = 0; count < program->num_words; count++)
    {
        sscanf (program->buff + pos, "%s%n", temp, &com_len);
        pos += com_len;
        
        check = strchr(temp, ':');
        if (check)
        {
            if (iter > 0)
                continue;
            MakeLable (lables, temp, check, &ofs, count + 1);    // count + 1 cause we counts from 0 and in debug user need from 1
        }
        #include "commands.h"
        else
        {
            printf ("Syntax Error!\n");
            printf ("In word: %s\n", temp);
            assert (!"OK");
        }
    }
    #undef DEF_CMD
    
    free (temp);
    free (check);
    
    printf ("\n%s\n", program->name);
    FILE* code = fopen(program->name, "wb+");   // Why not creating file when mode is "wb"?...
    fwrite (bin_buff, ofs, 1, code);
    fclose (code);
    
    iter++;
}

void MakeLable (lable* lables, char* temp, char* check, int* ofs, int count)
{
    int mode = -1;
    static int last_word_lable = 1;
    
    if (strcmp(check, ":") != 0)
    {
        printf ("\nSyntaxError!\n");
        printf ("Delete this \"%s\" from label %s (word %d)\n\n", check + 1, temp, count);
        assert (!"OK");
    }
    else
    {
        *(check) ='\0';
        if (strspn(temp, "1234567890") != strlen(temp))
        {
            mode = WORD;
            if (strspn(temp, "1234567890") != 0)
            {
                printf ("\nSyntaxError!\n");
                printf ("Those symbols in label must be an integer or word: %s (word %d)\n", temp, count);
                printf ("Don't mix!\n\n");
                assert (!"OK");
            }
        }
        else
        {
            mode = NUMBER;
        }
    }
    
    if (mode == NUMBER)
    {
        int cur_lable = atoi(temp);
        if (cur_lable <= last_word_lable)
        {
            while (lables[last_word_lable].adr != -1)
                last_word_lable++;
            assert (last_word_lable < MAX_NUM_LABLES);
            
            
            lables[last_word_lable].adr = lables[cur_lable].adr;
            strcpy (lables[last_word_lable].name, lables[cur_lable].name);
            strcpy (lables[cur_lable].name, "");
        }
        lables[cur_lable].adr = *ofs;
    }
    else if (mode == WORD)
    {
        while (lables[last_word_lable].adr != -1)
            last_word_lable++;
        assert (last_word_lable < MAX_NUM_LABLES);
        
        lables[last_word_lable].adr  =  *ofs;
        strcpy(lables[last_word_lable].name, temp);
    }
    else
    {
        printf ("\nError in MakeLable, cannot find out mode\n\n");
        assert (!"OK");
    }
}


int SearchLable (lable* lables, char* temp)
{
    for (int i = 0; i < MAX_NUM_LABLES + 1; i++)
    {
        if (strcmp(temp, lables[i].name) == 0)
        {
            return i;
        }
    }
    return -1;
}


void Sign_maker (int version, char* bin_buff, int* ofs)
{
    FileHeader Sign = {"IG", version};
    
    memcpy(bin_buff, &Sign, sizeof(Sign));
    *ofs += sizeof(Sign);
}


void ComplicComProcessing (char* bin_buff, int* ofs, char* temp, int count, lable* lables, int num)
{
    if (num == CMD_push)
    {
        PushProcessing (bin_buff, ofs, temp, count);
    }
    else if (num == CMD_pop)
    {
        PopProcessing  (bin_buff, ofs, temp, count);
    }
    else if ((CMD_jmp  <= num && num <=  CMD_jt) || num == CMD_call)
    {
        JmpProcessing  (bin_buff, ofs, temp, count, lables);
    }
}


void PushProcessing (char* bin_buff, int* ofs, char* temp, int count)
{
    
    if (strpbrk(temp, "1234567890"))
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
        
        int reg_number = FindRegNumber (temp, count);
        bin_buff[*ofs] = reg_number;
        *ofs += sizeof(char);
        printf ("%d", reg_number);
    }
}


void PopProcessing  (char* bin_buff, int* ofs, char* temp, int count)
{
    int reg_number = FindRegNumber (temp, count);
    bin_buff[*ofs] = reg_number;
    *ofs += sizeof(char);
    printf (" %d", reg_number);
}


void JmpProcessing (char* bin_buff, int* ofs, char* temp, int count, lable* lables)
{
    int mode = -1;
    
    if (temp[0] != ':')
    {
        printf ("\nSyntax Error!\n");
        printf ("Please enter lable in format \":x\", where x is integer (number of needed lable)\n");
        printf ("%s\n", temp);
        printf ("(in word %d)\n\n", count);
        assert (!"OK");
    }
    else
    {
        temp += 1;
        if (strspn(temp, "1234567890") != strlen(temp))
        {
            mode = WORD;
            if (strspn(temp, "1234567890") != 0)
            {
                printf ("\nSyntaxError!\n");
                printf ("Those symbols in label must be an integer or word: %s (word %d)\n", temp, count);
                printf ("Don't mix!\n\n");
                assert (!"OK");
            }
        }
        else
        {
            mode = NUMBER;
        }
    }
    
    int cur_lable = -1;
    if (mode == NUMBER)
    {
        cur_lable = atoi(temp);
        printf ("LABLE HERE, %d\n", cur_lable);
    }
    else if (mode == WORD)
    {
        cur_lable = SearchLable (lables, temp);
    }
    else
    {
        printf ("\nError in JmpProcessing, cannot find out mode\n\n");
        assert (!"OK");
    }
    if (cur_lable == -1)
        printf("ERRROR");
    
    *(int*)(bin_buff + *ofs) = (cur_lable == -1) ?  -1 : lables[cur_lable].adr;
    *ofs += sizeof(int);
    printf (" %d", lables[cur_lable].adr);
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
    else if (strcmp(temp, "cat") == 0)
    {
        return CAT;
    }
    else if (strcmp(temp, "myau") == 0)
    {
        return MYAU;
    }
    else
    {
        printf ("ERROR in word %d (%s)\n", count, temp);
        printf ("Name of register after push or pop doesn't match any of the existing register");
        printf ("\n\n(word is any symbol-number combination that you entered)");
        assert (!"OK");
    }
}
