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

CMD_JMP  = 13,
CMD_JA   = 14,
CMD_JAE  = 15,
CMD_JB   = 16,
CMD_JBE  = 17,
CMD_JE   = 18,
CMD_JNE  = 19,
CMD_JT   = 20,
CMD_CALL = 21,
CMD_RET  = 22
*/

DEF_CMD (HLT,  0, 0, 
                    {
                    processor->pc = code_size;    
                    })

DEF_CMD (IN,   1, 0, 
                    {
                    int number = 0;
                    scanf ("%d", &number);
                    StackPush (processor->stack, number);
                    processor->pc++;
                    })

DEF_CMD (OUT,  2, 0, 
                    {
                    printf ("\n%lg\n\n", StackPop (processor->stack));
                    processor->pc++;
                    })

DEF_CMD (PUSH, 3, 2, 
                    {
                    if (processor->code[processor->pc + 1] == 1)
                        {
                            StackPush (processor->stack, *(double*)(processor->code + processor->pc + 2));
                            processor->pc += 2*sizeof(char) + sizeof(double);
                        }
                    else if (processor->code[processor->pc + 1] == 2)
                        {
                            StackPush (processor->stack, processor->registers[processor->code[processor->pc + 2]]);
                            processor->pc += 3*sizeof(char);
                        }
                    else
                        {
                            printf ("You've got biiig troubles, my dear)");
                            printf ("\nYour bite-code is incorrect");
                            assert (!"OK");
                        }
                    })

DEF_CMD (POP,  4, 2, 
                    {
                    double temp = StackPop (processor->stack);
                    processor->registers[processor->code[processor->pc + 1]] = temp;
                    processor->pc += 2*sizeof(char);
                    })

DEF_CMD (ADD,  5, 0, 
                    {
                    StackPush (processor->stack, StackPop(processor->stack) + StackPop(processor->stack));
                    processor->pc++;
                    })

DEF_CMD (SUB,  6, 0, 
                    {
                    double temp = StackPop (processor->stack);
                    StackPush (processor->stack, StackPop(processor->stack) - temp);
                    processor->pc++;
                    })

DEF_CMD (MULT, 7, 0, 
                    {
                    StackPush (processor->stack, StackPop(processor->stack) * StackPop(processor->stack));
                    processor->pc++;
                    })

DEF_CMD (SEGM, 8, 0, 
                    {
                    double temp = StackPop (processor->stack);
                    if (temp != 0)
                        StackPush (processor->stack, StackPop(processor->stack) / temp);
                    else
                        {
                            printf ("Trying to segment by zero");
                            assert (!"OK");
                        }
                    processor->pc++;
                    })

DEF_CMD (POW,  9, 0, 
                    {
                    int temp = StackPop(processor->stack);
                    StackPush (processor->stack, pow(StackPop(processor->stack), temp));
                    processor->pc++;
                    })

DEF_CMD (SQRT, 10, 0, 
                    {
                    StackPush (processor->stack, sqrt(StackPop(processor->stack)));
                    processor->pc++;
                    })

DEF_CMD (SIN, 11, 0, 
                    {
                    StackPush (processor->stack, sin(StackPop(processor->stack)));
                    processor->pc++;
                    })

DEF_CMD (COS, 12, 0, 
                    {
                    StackPush (processor->stack, cos(StackPop(processor->stack)));
                    processor->pc++;
                    })

DEF_CMD (JMP, 13, 1, 
                    {
                    processor->pc = *(int*)(processor->code + processor->pc + 1);
                    })

DEF_CMD (JA,  14, 1, 
                    {
                    int temp1 = StackPop(processor->stack);
                    int temp2 = StackPop(processor->stack);
                    if (temp2 > temp1)
                        {
                            processor->pc = *(int*)(processor->code + processor->pc + 1);
                        }
                    else
                        {
                            processor->pc += sizeof(char) + sizeof(int);
                        }
                    })

DEF_CMD (JAE, 15, 1, 
                    {
                    int temp1 = StackPop(processor->stack);
                    int temp2 = StackPop(processor->stack);
                    if (temp2 >= temp1)
                        {
                            processor->pc = *(int*)(processor->code + processor->pc + 1);
                        }
                    else
                        {
                            processor->pc += sizeof(char) + sizeof(int);
                        }
                    })

DEF_CMD (JB,  16, 1, 
                    {
                    int temp1 = StackPop(processor->stack);
                    int temp2 = StackPop(processor->stack);
                    if (temp2 < temp1)
                        {
                            processor->pc = *(int*)(processor->code + processor->pc + 1);
                        }
                    else
                        {
                            processor->pc += sizeof(char) + sizeof(int);
                        }
                    })

DEF_CMD (JBE, 17, 1, 
                    {
                    int temp1 = StackPop(processor->stack);
                    int temp2 = StackPop(processor->stack);
                    if (temp2 <= temp1)
                        {
                            processor->pc = *(int*)(processor->code + processor->pc + 1);
                        }
                    else
                        {
                            processor->pc += sizeof(char) + sizeof(int);
                        }
                    })

DEF_CMD (JE,  18, 1, 
                    {
                    int temp1 = StackPop(processor->stack);
                    int temp2 = StackPop(processor->stack);
                    if (temp2 == temp1)
                        {
                            processor->pc = *(int*)(processor->code + processor->pc + 1);
                        }
                    else
                        {
                            processor->pc += sizeof(char) + sizeof(int);
                        }
                    })

DEF_CMD (JNE, 19, 1, 
                    {
                    int temp1 = StackPop(processor->stack);
                    int temp2 = StackPop(processor->stack);
                    if (temp2 != temp1)
                        {
                            processor->pc = *(int*)(processor->code + processor->pc + 1);
                        }
                    else
                        {
                            processor->pc += sizeof(char) + sizeof(int);
                        }
                    })
DEF_CMD (JT,  20, 1, 
                    {
                    
                    })

DEF_CMD (CALL, 21, 1,
                    { 
                    StackPush (processor->calls, processor->pc + sizeof(char) + sizeof(int));
                    processor->pc = *(int*)(processor->code + processor->pc + 1);
                    })

DEF_CMD (RET,  22, 0, 
                    {
                    processor->pc = StackPop(processor->calls);
                    })

DEF_CMD (ERR, 23, 0,
                    {
                    printf("Discriminant < 0\n");
                    processor->pc += sizeof(char);
                    })

DEF_CMD (NOROOTS, 24, 0,
                    {
                    printf("No roots\n");
                    processor->pc += sizeof(char);
                    })

DEF_CMD (INFROOTS, 25, 0,
                    {
                    printf("INF roots\n");
                    processor->pc += sizeof(char);
                    })

DEF_CMD (START, 26, 0,
                    {
                    processor->pc = *(int*)(processor->code + processor->pc + 1);
                    })
//!!!!
// If add something, don't forget to update NUM_COMANDS constant
//!!!!
