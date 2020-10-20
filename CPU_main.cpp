#include "CPU_header.h"

int main()
{
    struct text program = {0, NULL, NULL};
    program.name = (char*) calloc (MAX_FILENAME + 1, sizeof(char));       // + 1 for converting .txt into .code
    assert (program.name);
    printf ("Please, enter name of the TXT file with your programm: ");
    scanf  ("%s", program.name);
    
    if (strlen(program.name) > MAX_FILENAME)
    {
        printf ("\nYour file name should be no longer than %d symbols\n\n", MAX_FILENAME);
        assert(!"OK");
    }
    
    Compiler (&program);
    
    free(program.name);
    free(program.ptr);
    
    return 0;
}


void Compiler (struct text* program)
{   
    
    size_t num_symb = TextGetter (program);
    program->num_lines = LinesCounter (program->ptr, num_symb);

    NameProcessing (program);
    
    CodeMaker (program, num_symb);
    
}


void NameProcessing (text* program)
{
    int len = strlen(program->name);
    assert(program->name[len-4] == '.');
    
    const char* extension = ".code";
    assert(extension[0] == '.');
    
    for (int i = len-4, j = 0; i <= len; i++, j++)
    {
        program->name[i] = extension[j];
    }
    
    printf ("\nYou can see bite-code of your program in file: %s\n", program->name);
}


size_t TextGetter (text* program)
{
    size_t size = SizeGetter (program->name);
    program->ptr = (char*) calloc (size + 1, sizeof(char));
    assert (program->ptr != NULL);
    
    size_t num_symbals = TextReader (program, size);
    assert (num_symbals != NULL);
    
    return num_symbals;
}


size_t SizeGetter (const char* name)
{
    FILE* file = fopen (name, "r");
    
    struct stat finfo;
    stat(name, &finfo);
    size_t size = finfo.st_size;
        
    fclose (file);
    
    return size;
}


size_t TextReader (text* program, size_t size)
{
    FILE* file = fopen (program->name, "r");
    
    size_t num_symb = fread (program->ptr, sizeof(char), size, file);
    *(program->ptr + num_symb) = '\0';
    
    fclose (file);
    
    return num_symb;
}


size_t LinesCounter (char* text, size_t num_symbals)
{
    assert (text != 0);
    size_t num_lines = 1;
    
    for (int i = 0; i < num_symbals - 1; i++)
        if ( *(text + i) == '\n' || *(text + i) == ' ')               
            {
            *(text + i) = '\0';
            if (*(text + i + 1) != '\n' && *(text + i) != ' ')
                num_lines++;
            }
    
    return num_lines;
}


void CodeMaker (text* program, size_t size)
{
    FILE* file = fopen (program->name, "wb");
    char* word = (char*) calloc (MAX_COMAND_LENGTH, sizeof(char));
    char* pointer = program->ptr;
    int   pos  =  0;

    int   check_lines = -1;

    for (int i = 0; i < program->num_lines; i++)
    {
        sscanf (pointer, "%s%n", word, &pos);
        pointer += pos + 1;
        
        check_lines = ComandsConverter (word, file);
    }
    if (check_lines != program->num_lines)
    {
        printf ("INPUT ERROR: Bad format, please, enter your program in that way:\n");
        assert (!"OK");
    }

    fclose (file);
}


int ComandsConverter (char* word, FILE* bite_code)
{
    char*   comand_array[] = {"hlt", "in", "out", "push", "pop", "add", "sub", "mul", "segm", "pow", "sqrt", "sin", "cos"};
    char* register_array[] = {"", "rax", "rbx", "rcx", "rdx"};
    
    static int line = 1;
    
    static int last_comand = -1;
    int t = -1;
    int i =  1;
    
    for (t = -1; t < NUM_COMANDS; t++)
    {
        if (strpbrk (word, "1234567890") || (t >= 0 && strcmp (word, comand_array[t]) == 0))
            break;
    }
    for (i = 1; i <= NUM_REGISTERS; i++)
    {
        if (strcmp (word, register_array[i]) == 0)
            break;
    }
    
    CheckErrors(t, i, last_comand, line)
    
    if (t == -1)
    {
        if (last_comand == PUSH)
            fprintf (bite_code, " 1 ");
        fprintf (bite_code, "%d\n", atoi(word));
        line++;
    }
    else
        if (last_comand == PUSH)
        {
            fprintf (bite_code, " 2 ");
            fprintf (bite_code, "%d\n", i);
            line++;
        }
        else
        {   
            fprintf (bite_code, "%d", t);
            if (t != PUSH && t != POP)
            {
                fprintf (bite_code, "\n");
                line++;
            }
        }
    
    last_comand = t;
    
    return line;
}
