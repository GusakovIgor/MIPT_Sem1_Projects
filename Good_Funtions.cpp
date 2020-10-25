// Previous versions of some functions, that might be good


int ComandsConverter (char* word, FILE* bite_code)
{
    char*   comand_array[] = {"hlt", "in", "out", "push", "pop", "add", "sub", "mul", "segm", "pow", "sqrt", "sin", "cos"};
    char* register_array[] = {"", "rax", "rbx", "rcx", "rdx"};
    
    static int line = 1;
    static int last_comand = -1;
    
    int com_num = -1;
    int reg_num =  1;
    
    for (com_num = -1; com_num < NUM_COMANDS; com_num++)
    {
        if (strpbrk (word, "1234567890") || (com_num >= 0 && strcmp (word, comand_array[com_num]) == 0))
            break;
    }
    for (reg_num = 1; reg_num <= NUM_REGISTERS; reg_num++)
    {
        if (strcmp (word, register_array[reg_num]) == 0)
            break;
    }
    
    CheckErrors(com_num, reg_num, last_comand, line)
    
    if (com_num == -1)
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
            fprintf (bite_code, "%d\n", reg_num);
            line++;
        }
        else
        {   
            fprintf (bite_code, "%d", com_num);
            if (com_num != PUSH && com_num != POP)
            {
                fprintf (bite_code, "\n");
                line++;
            }
        }
        
    last_comand = com_num;
    
    return line;
}
