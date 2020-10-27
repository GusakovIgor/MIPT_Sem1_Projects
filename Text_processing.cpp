#include "CPU_header.h"

text* ProgramConstructor (struct text* program)
{   
    
    size_t num_symb = TextGetter (program);
    program->num_words = WordsCounter (program->buff, num_symb);
    program->name = NameProcessing (program->name);
    
    return program;
}


char* NameProcessing (char* name)
{
    int len = strlen(name);
    assert (name[len-4] == '.');
    
    const char* extension = ".bin";
    assert (extension[0] == '.');
    
    for (int i = len-4, j = 0; i <= len-1; i++, j++)
    {
        name[i] = extension[j];
    }
    
    printf ("\nYou can see byte-code of your program in file: %s\n", name);

    return name;
}


size_t TextGetter (text* program)
{
    size_t size = SizeGetter (program->name);
    program->buff = (char*) calloc (size + 1, sizeof(char));
    assert (program->buff != NULL);
    
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
    
    size_t num_symb = fread (program->buff, sizeof(char), size, file);
    *(program->buff + num_symb) = '\0';
    
    fclose (file);
    
    return num_symb;
}


size_t WordsCounter (char* text, size_t num_symbals)
{
    assert (text != 0);
    size_t num_words = 1;
    
    for (int i = 0; i < num_symbals - 1; i++)
        while (isspace(text[i]))
        {
            if (!isspace(text[i + 1]))
            {
                num_words++;
                break;
            }
            else
                i++;
        }
    
    return num_words;
}


