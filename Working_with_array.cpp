#include "OneginHeader.h"

void Text_Getter (const char* name, arr_info* text)
{
    size_t size = Size_Getter(name);
    (*text).arr_ptr = (char*) calloc(size + 1, sizeof(char));
    assert ((*text).arr_ptr != NULL);
    (*text).num_sym = Text_Reader((*text).arr_ptr, size, name);
    assert ((*text).num_sym != NULL);
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
//----------------------------------------------------------------------
size_t Text_Reader(char* text, size_t size, const char* name)
    {
    FILE* Onegin = fopen(name, "r");
    
    size_t num_symb = fread(text, sizeof(char), size, Onegin);
    *(text + num_symb) = '\0';
    
    fclose(Onegin);
    
    return num_symb;
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
size_t Lines_Counter(char* text, size_t num_symbals)
    {
    assert (text != 0);
    size_t num_lines = 1;
    
    for (int i = 0; i < num_symbals - 1; i++)
        if ( *(text + i) == '\n')               
            {
            *(text + i) = '\0';
            if (*(text + i + 1) != '\n')
                num_lines++;
            }
    
    return num_lines;
    }


//----------------------------------------------------------------------
//! ZeroSkipper function:
//!     1) Skips all the '\0' symbals from current position in text
//!     2) Returning pointer on first not '\0' symbal
//!
//! @param [out]  text - array we are working with
//! @param [in]   i    - number (in array) of symbal we need to skip from
//!
//! @return       i    - number (in array) of the first not '\0' symbal
//!
//----------------------------------------------------------------------
size_t ZeroSkipper (char* text, size_t i)
    {
    while ( *(text + i) == '\0')
        i++;
    return i;
    }


//----------------------------------------------------------------------
//! This function makes and returns an array of structures,
//! each of which contain information about one string in text array:
//!     1) It creates a structure array - Arr, with num_lines size (number of lines in text), using calloc
//!
//!     2) Creates sime variables:
//!         a) line_counter - iterator, that using for counting, which line in text we processing now
//!         b) flag         - variable, that is 1 if we're in line and 0 if we're get out of it
//!
//!     3) Than in cycle for every symbal if we're in line:
//!         a) if symbal is '\0' makes this symbal end of line, which we're in now, calculates its size, and increases counter of lines
//!         b) then (if it's not last line) resets flag and using ZeroSkipper to skip all the consecutive '\0'
//!                                       if we're not in line:
//!         c) that means that we're on the first not '\0' symbal after end of line
//!            so function makes it the begining of next line and puts flag in 1
//!     
//!     4) After all these actions checks that counter of lines matches num_lines
//!
//!
//! @param [out] text        - array we are working with
//! @param [in]  num_symbals - amount of symbals in that array
//! @param [in]  num_lines   - amount of Not Empty lines in array we're working with
//!
//! @return      Arr         - pointer on the beginig of array with structures
//!
//----------------------------------------------------------------------
struct str_info* Struct_Array_Maker(char* text, size_t num_symbols, size_t num_lines)
    {
    struct str_info* Arr = (struct str_info*) calloc (num_lines, sizeof(struct str_info));
    assert (Arr != NULL);
    
    size_t line_counter = 0;
    int flag = 0;
    
    for (size_t i = ZeroSkipper (text, 0); i < num_symbols + 1; i++)
        {  
        if (flag && *(text + i) == '\0')
            {
            (*(Arr + line_counter)).end_ptr = text + i;
            (*(Arr + line_counter)).size = (*(Arr + line_counter)).end_ptr - (*(Arr + line_counter)).ptr;
            line_counter++;
            
            if (line_counter < num_lines)
                {
                flag = 0;
                i = ZeroSkipper (text, i);
                }
            }
        
        assert(*(text + i) != '\0' || i == num_symbols);
        
        if (!flag)
            {
            (*(Arr + line_counter)).ptr = text + i;
            flag = 1;
            }
        }
    assert (line_counter == num_lines);
    
    return Arr;
    }


//----------------------------------------------------------------------
//! Memory_Cleaner function:
//!     1) gets num_arg
//!     2) for every arg, doing free()
//!
//! @param [in]  num_arg - namber of arguments, that will be in function (without first argument)
//!
//! @return void
//!
//----------------------------------------------------------------------   
void Memory_Cleaner (int num_arg, ...)
{
    va_list ap;
    va_start(ap, num_arg);
    
    for (int i = 0; i < num_arg; i++)
    {
        free (va_arg(ap, void*));
    }
    
    va_end(ap);
}


//----------------------------------------------------------------------
//! FileNuller function:
//!     1) Opens file in "writing" mode
//!     2) Closing this file
//!
//! @param [in]  file_name - name of file, we needed to null
//!
//! @return void
//!
//----------------------------------------------------------------------    
void FileNuller (const char* file_name)
{
    FILE* file = fopen (file_name, "w");
    fclose(file);
}



void Text_Writer (const char* DistFileName, size_t num_lines, struct str_info* struct_array, int mode)
{
    assert (mode == 1 || mode == 2);
    
    FILE* DistFile = fopen(DistFileName, "a");
    
    if(mode == 1)
    {
        for (size_t i = 0; i <= num_lines-1; i++)
        {
        fprintf (DistFile, "%s\n", (*(struct_array + i)).ptr);
        }
    }
    else if (mode == 2)
    {
        for (size_t i = 0; i <= num_lines-1; i++)
        {
        fprintf (DistFile, "%40s\n", (*(struct_array + i)).ptr);
        }
    }
    
    fprintf(DistFile, "-------------------------------------------------------------------------------------------------------------------\n");
    
    fclose(DistFile);
}
