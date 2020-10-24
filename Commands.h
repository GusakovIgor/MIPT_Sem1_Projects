/* Commands.h

CMD_HLT  = 0, 
CMD_IN   = 1, 
CMD_OUT  = 2, 
CMD_PUSH = 3, 
CMD_POP  = 4, 
CMD_ADD  = 5, 
CMD_SUB  = 6, 
CMD_MULT = 7, 
CMD_SEGM = 8, 
CMD_POW  = 9, 
CMD_SQRT = 10, 
CMD_SIN  = 11, 
CMD_COS  = 12,

*/

DEF_CMD (hlt,  0, 0, 
                    {
                        
                    })

DEF_CMD (in,   1, 0, 
                    {
                    int number = 0;
                    scanf ("%d", &number);
                    StackPush (processor->stack, number);
                    })

DEF_CMD (out,  2, 0, 
                    {
                    printf ("%d", StackPop(&stk));
                    })

DEF_CMD (push, 3, 2, 
                    {
                    if (code[processor->pc + 1] == 1)
                        {
                            StackPush (processor->stack, code[processor->pc + 2]);
                            pc += 2;
                        }
                    else if (code[processor->pc + 1] == 2)
                        {
                            StackPush (processor->stack, processor->registers[code[processor->pc + 2]]);
                            pc += 2;
                        }
                    else
                        {
                            printf ("You've got biiig troubles, my dear)");
                            printf ("\nYour bite-code is incorrect");
                            assert (!"OK");
                        }
                    })

DEF_CMD (pop,  4, 2, 
                    {
                    double temp = StackPop(processor->stack);
                    processor->registers[code[processor->pc + 1]] = temp;
                    pc += 1;
                    })

DEF_CMD (add,  5, 0, 
                    {
                    StackPush (processor->stack, StackPop(processor->stack) + StackPop(processor->stack));
                    })

DEF_CMD (sub,  6, 0, 
                    {
                    
                    })

DEF_CMD (mult, 7, 0, 
                    {
                    
                    })

DEF_CMD (segm, 8, 0, 
                    {
                    
                    })

DEF_CMD (pow,  9, 0, 
                    {
                    
                    })

DEF_CMD (sqrt, 10, 0, 
                    {
                    
                    })

DEF_CMD (sin, 11, 0, 
                    {
                    
                    })

DEF_CMD (cos, 12, 0, 
                    {
                    
                    })
