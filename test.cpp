#include "CPU_header.h"

int main ()
{
    char* number = "push 4";
    char* text   = "push rdx";
    
    char* test = (char*) calloc (10, sizeof(char));
    sscanf (number, "%s", test);
    printf ("number = %s", test);
    
    
    return 0;
}
