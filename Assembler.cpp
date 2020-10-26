#include "CPU_header.h"

int main (int argc, const char* argv[])
{
    text* program = (text*) calloc (1, sizeof(text));
    
    program->name = (char*) calloc (MAX_FILENAME, sizeof(char));
    program->name = (char*) argv[1];
    assert (strlen(program->name) < MAX_FILENAME);
    
    program = ProgramConstructor (program);
    
    
    lable*  lables = (lable*)  calloc (MAX_NUM_LABLES + 1, sizeof(lable));      // Plus 1 because user probably want to use labels 1-50
    for (int i = 0; i < MAX_NUM_LABLES + 1; i++)                                // if MAX_NUM_LABELS = 50, so we've got labels 0-50
    {
        lables[i].adr = -1;
    }
    
    for (int i = 0; i < NUM_ASM; i++)
    {
        Assembler (program, lables, 4);
    }
    
    free (lables);
    
    free (program->name);
    free (program->buff);
    free (program);
    
    return 0;
}


void Assembler (text* program, lable* lables, short version)
{
    char* bin_buff = (char*) calloc (MAX_CODE_LEN,   sizeof(char));
    char* temp     = (char*) calloc (MAX_COMAND_LEN, sizeof(char));
    char* check    = (char*) calloc (MAX_COMAND_LEN, sizeof(char));
    
    int ofs = 0;
    int pos = 0;
    int com_len = 0;
    int reg_number = 0;
    
    static int iter = 0;
    
    //Sign_maker (version, bin_buff, &ofs);
    
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
                PushProcessing (bin_buff, &ofs, temp, count);               \
            }                                                               \
            else if (num == CMD_pop)                                        \
            {                                                               \
                count++;                                                    \
                sscanf (program->buff + pos, "%s%n", temp, &com_len);       \
                pos += com_len;                                             \
                                                                            \
                PopProcessing (bin_buff, &ofs, temp, count);                \
            }                                                               \
            else if ((CMD_jmp  <= num && num <=  CMD_jt) || num == CMD_call)\
            {                                                               \
                count++;                                                    \
                sscanf (program->buff + pos, "%s%n", temp, &com_len);       \
                pos += com_len;                                             \
                                                                            \
                JmpProcessing (bin_buff, &ofs, temp, count, lables);        \
            }                                                               \
            printf("\n");   \
        }                                                                   \
    
    for (int count = 0; count < program->num_words; count++)
    {
        sscanf (program->buff + pos, "%s%n", temp, &com_len);
        pos += com_len;
        
        check = strchr(temp, ':');
        if (check)
        {
            MakeLable (lables, temp, check, ofs, count + 1);    // count + 1 cause we counts from 0 and in debug user need from 1
            if (iter > 0)
                continue;
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
    
    FILE* code = fopen(program->name, "w+b");   // Why not creating file when mode is "wb"?...
    fwrite (bin_buff, ofs, 1, code);
    fclose(code);
    
    free (bin_buff);
    
    iter++;
}

void MakeLable (lable* lables, char* temp, char* check, int ofs, int count)
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
        if (cur_lable < last_word_lable)
        {
            while (lables[last_word_lable].adr != -1)
                last_word_lable++;
            lables[last_word_lable].adr = lables[cur_lable].adr;
            strcpy(lables[last_word_lable].name, lables[cur_lable].name);
        }
        lables[cur_lable].adr = ofs;
    }
    else if (mode == WORD)
    {
        while (lables[last_word_lable].adr != -1)
            last_word_lable++;
        lables[last_word_lable].adr  =  ofs;
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

//Debug It
void Sign_maker (short version, char* bin_buff, int* ofs)
{
    FileHeader Sign = {"IG", version};
    
    *(FileHeader*)(bin_buff + *ofs) = Sign;
    *ofs += sizeof(Sign);
}
//-------------------------------------------------------

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
    
    //printf("MODE = %d", mode);
    //printf("cur_lable = %d", cur_lable);
    *(int*)(bin_buff + *ofs) = lables[cur_lable].adr;
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
    else
    {
        printf ("ERROR in word %d\n", count);
        printf ("Name of register after push or pop doesn't match any of the existing register");
        printf ("\n\n(word is any symbol-number combination that you entered)");
        assert (!"OK");
    }
}
