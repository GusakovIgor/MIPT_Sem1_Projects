#include "OneginHeader.h"

void Unit_Tests (int argc, const char* argv[])
{
    assert(argv != 0);
    assert(argc  > 1);
    
    if (strcmp(*(argv + 1), "All") == 0)
    {
        printf("All Unit_tests starting\n");
    }
    else if (strcmp(*(argv + 1), "None") == 0)
    {
        printf("Bad student, use Unit_tests next time\n");
        Help_Print(OK);
    }
    else if (strpbrk((*(argv + 1)), "123456789") != 0)
    {
        for (int i = 1; i <= argc; i++)
        {
            switch (atoi(*(argv + i)) )
            {
                case (1):   printf("Size_Getter_Test:\n");
                            Size_Getter_Test();
                            break;
                case (2):   printf("Lines_Counter_Test:\n");
                            Lines_Counter_Test();
                            break;
                case (3):   printf("Unit_3\n");
                            break;
                case (4):   printf("Unit_4\n");
                            break;
                case (5):   printf("Unit_5\n");
                            break;
            }
        }
    }
    else
        Help_Print(ERROR);
}

void Help_Print(int error)
{
    if (error < 0) 
    {
        printf("Input error. Please enter number of the test, you want to be prformed:\n");
    }
    printf("\"1\" Size_Getter_Test\n");
    printf("\"2\" Lines_Counter_Test\n");
    printf("\"3\" is Unit_3\n");
    printf("\"4\" is Unit_4\n");
    printf("\"5\" is Unit_5\n");
    printf("\n\"All\" if you need to do all tests at a time\n");
    printf("\"None\" if you don't want to do tests\n");
}

void Size_Getter_Test ()
{
    const int num_tests = 5;
    int exp_size[num_tests] = {7, 4, 0, 26, 22};
    
    char file_name[16] = "Test_size_i.txt";
    size_t len = strlen(file_name);
    
    size_t size = 0;
    
    
    for (int i = 1; i <= num_tests; i++)
    {
        file_name[len-5] = '0' + i;
        file_name[len] = '\0';
        
        size = Size_Getter (file_name);
        
        if ((size) == *(exp_size + i - 1))
        {
            printf("\tTest %d is Good (File %s is %d bytes)\n", i, file_name, size);
        }
        else
        {
            printf("\tTest %d is Bad\n", i);
            printf("\tExpected %d bytes, but Size_Getter finds %d\n", *(exp_size + i - 1), size);
        }
    }
}


void Lines_Counter_Test ()
{
    const int num_tests = 5;
    int exp_num_lines[num_tests] = {1, 3, 4, 2, 1};
    
    char file_name[16] = "Test_line_i.txt";
    size_t len = strlen(file_name);
    
    size_t num_lines = 0;
    
    
    struct arr_info test;
    for (int i = 1; i <= num_tests; i++)
    {
        Text_Getter (file_name, &test);
        file_name[len-5] = '0' + i;
        file_name[len] = '\0';
        
        //printf("%s\n", file_name);
        num_lines = Lines_Counter(test.arr_ptr, test.num_sym); // Don't work here
        //printf("%s\n is OK", file_name);
        
        if ((num_lines) == *(exp_num_lines + i - 1))
        {
            printf("\tTest %d is Good (File %s has %d lines)\n", i, file_name, num_lines);
        }
        else
        {
            printf("\tTest %d is Bad\n", i);
            printf("\tExpected %d lines, but Lines_Counter finds %d\n", *(exp_num_lines + i - 1), num_lines);
        }
    }
}
