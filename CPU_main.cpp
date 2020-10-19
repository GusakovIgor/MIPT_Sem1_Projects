#include "CPU_header.h"

int main()
{
    struct text program = {0, NULL, NULL};
    program.name = (char*) calloc (MAX_FILENAME + 1, sizeof(char));       // + 1 for converting .txt into .code
    printf("Please, enter name of the TXT file with your programm:\n");
    scanf ("%s", program.name);
    
    if (strlen(program.name) > MAX_FILENAME)
    {
        printf ("\nYour file name should be no longer than %d symbols\n\n", MAX_FILENAME);
        assert(!"OK");
    }
    
    
    //char* program_name_old = program_name;
    Compiler (&program);
    
    free(program.name);
    free(program.ptr);
    
    return 0;
}


void Compiler (struct text* program)
{   
    
    program->size = Size_Getter(program->name);
    
    program->ptr = (char*) calloc (program->size + 1, sizeof(char));
    assert (program->ptr != NULL);
    
    program->size = Text_Reader(program);
    
    // NAME PROCESSING
    NameProcessing (program);
    //NAME PROCESSING
    
    CodeMaker (program);
    
}

//----------------------------------------------------------------------
//! Size_Getter function: finds out size of file in bytes and returns it
//!     1) Opening file
//!     2) Declaring a standart struct of type finfo, that will contain info about our file
//!     3) Using this structure getting the file size, and returning them
//!
//! @param [in]  name - name (const char*) of the file, which size we need to know
//!
//! @return      size - size (in bytes) of the file, which size we need to know
//!
//----------------------------------------------------------------------
size_t Size_Getter (const char* name)
    {
    FILE* Onegin = fopen(name, "r");
    
    struct stat finfo;
    stat(name, &finfo);
    size_t size = finfo.st_size;
        
    fclose(Onegin);
    
    return size;
    }

//----------------------------------------------------------------------
//! Text_Reader function: reads text in an array
//!     1) It opens file and using fread() writing whole Onegin file in text array
//!     2) fread() returns amount of symbals in Onegin, so function writes it in num_symb
//!     3) Then function closes file Onegin and returns num_symb
//!
//! @param [out]  text - array we are working with
//! @param [in]   size - size (in bytes) of the file, which we need to read
//! @param [in]   name - name (const char*) of the file, which we need to read
//!
//! @return       num_symb - amount of symbals in text (including \n \r and other service symbals)
//!
//char* text, size_t size, const char* name
//----------------------------------------------------------------------
size_t Text_Reader (text* program)
{
    FILE* text = fopen(program->name, "r");
    
    size_t num_symb = fread (program->ptr, sizeof(char), program->size, text);
    *(program->ptr + num_symb) = '\0';
    
    fclose(text);
    
    return num_symb;
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
    
    printf ("\n%s", program->name);
}


//----------------------------------------------------------------------
//! Lines_Counter function: counting Not Empty lines in text and deleting '\n' from it
//!     1) It gets array of char
//!     2) Runs through all elements in it
//!     3) If element is '\n' changing it on '\0'
//!     4) And if it is also the first of consecutive (подряд идущих) '\n' function counts it
//!
//! @param [out]  text        - array we are working with
//! @param [in]   num_symbals - amount of symbals in that array
//!
//! @return       num_lines   - amount of Not Empty lines in array we're working with
//!
//----------------------------------------------------------------------
void CodeMaker (text* program)
{
    FILE* bite_code = fopen (program->name, "w");
    assert (program->ptr != 0);
    
    char last_ok = '0';
    char* word = (char*) calloc (MAX_COMAND_LENGTH, sizeof(char));
    int iter = 0;
    int count_lines = 0;
    bool in_word = false;
    
    for (int i = 0; i < program->size; i++)
    {
        if ( *(program->ptr + i) == '\n' && *(program->ptr + i + 1) != '\n')               // '\n' puts only when we need them
        {
            if (in_word)
            {
                count_lines++;
                ComandsConverter(word, bite_code, count_lines);
                    
                printf("\n%s", word);
                in_word = false;
            }
            fputc(*(program->ptr + i), bite_code);
            last_ok = *(program->ptr + i);
        }
        else if (*(program->ptr + i) == ' ' && i != 0 && isalnum(*(program->ptr + i + 1)) && isalnum(last_ok))            //  ' ' puts only when we need them
        {
            if (in_word)
            {
                ComandsConverter(word, bite_code, count_lines);
                printf("\n%s", word);
                in_word = false;
            }
            fputc(*(program->ptr + i), bite_code);
            last_ok = *(program->ptr + i);
        }
        else if (isalnum(*(program->ptr + i)))
        {
            if (!in_word)
                in_word = true;
            
            word[iter++] = *(program->ptr + i);
            last_ok = *(program->ptr + i);
        }
        
        // Free word if we ended with current one
        if (!in_word)
        {
            word[iter] = 0;
            while (iter > 0)
            {
                word[--iter] = 0;
            }
        }
        //
        
    }
    
    if (in_word)
    {
        ComandsConverter(word, bite_code, count_lines);
        printf ("\n%s", word);
        in_word = false;
    }
    
    free (word);
    
    fclose (bite_code);
}

void ComandsConverter (char* word, FILE* bite_code, int count_lines)
{
    char* comand_array[] = {"hlt", "in", "out", "push", "add", "sub", "mult", "segm", "pow", "sqrt", "sin", "cos"};
    int t = -1;
    for (; t < NUMBER_OF_COMANDS; t++)
    {
        if (t > 0 && strcmp (word, comand_array[t]) == 1 || strpbrk(word, "1234567890"))
        {
            break;
        }
    }
    
    if (t == NUMBER_OF_COMANDS)
    {
        printf ("\nBroken word is \"%s\"\n", word);
        printf ("\nPlease, check program. Your comand on line %d doesn't match any of existing ones\n", count_lines);
        assert (!"OK");
    }
    else
    {
        fprintf (bite_code, "%d", word);
    }
}
