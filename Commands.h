/* Commands.h

CMD_hlt  = 0, 
CMD_in   = 1, 
CMD_out  = 2, 
CMD_push = 3, 
CMD_pop  = 4, 
CMD_add  = 5, 
CMD_sub  = 6, 
CMD_mult = 7, 
CMD_segm = 8, 
CMD_pow  = 9, 
CMD_sqrt = 10, 
CMD_sin  = 11, 
CMD_cos  = 12,

CMD_jmp  = 13,
CMD_ja   = 14,
CMD_jae  = 15,
CMD_jb   = 16,
CMD_jbe  = 17,
CMD_je   = 18,
CMD_jne  = 19,
CMD_jt   = 20,
CMD_call = 21,
CMD_ret  = 22
*/

DEF_CMD (hlt,  0, 0, 
                    {
                    processor->pc = code_size;    
                    })

DEF_CMD (in,   1, 0, 
                    {
                    int number = 0;
                    scanf ("%d", &number);
                    StackPush (processor->stack, number);
                    processor->pc++;
                    })

DEF_CMD (out,  2, 0, 
                    {
                    printf ("\n%lg\n\n", StackPop (processor->stack));
                    processor->pc++;
                    })

DEF_CMD (push, 3, 2, 
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

DEF_CMD (pop,  4, 2, 
                    {
                    double temp = StackPop (processor->stack);
                    processor->registers[processor->code[processor->pc + 1]] = temp;
                    processor->pc += 2*sizeof(char);
                    })

DEF_CMD (add,  5, 0, 
                    {
                    StackPush (processor->stack, StackPop(processor->stack) + StackPop(processor->stack));
                    processor->pc++;
                    })

DEF_CMD (sub,  6, 0, 
                    {
                    double temp = StackPop (processor->stack);
                    StackPush (processor->stack, StackPop(processor->stack) - temp);
                    processor->pc++;
                    })

DEF_CMD (mult, 7, 0, 
                    {
                    StackPush (processor->stack, StackPop(processor->stack) * StackPop(processor->stack));
                    processor->pc++;
                    })

DEF_CMD (segm, 8, 0, 
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

DEF_CMD (pow,  9, 0, 
                    {
                    int temp = StackPop(processor->stack);
                    StackPush (processor->stack, pow(StackPop(processor->stack), temp));
                    processor->pc++;
                    })

DEF_CMD (sqrt, 10, 0, 
                    {
                    StackPush (processor->stack, sqrt(StackPop(processor->stack)));
                    processor->pc++;
                    })

DEF_CMD (sin, 11, 0, 
                    {
                    StackPush (processor->stack, sin(StackPop(processor->stack)));
                    processor->pc++;
                    })

DEF_CMD (cos, 12, 0, 
                    {
                    StackPush (processor->stack, cos(StackPop(processor->stack)));
                    processor->pc++;
                    })

DEF_CMD (jmp, 13, 1, 
                    {
                    processor->pc = *(int*)(processor->code + processor->pc + 1);
                    })

DEF_CMD (ja,  14, 1, 
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

DEF_CMD (jae, 15, 1, 
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

DEF_CMD (jb,  16, 1, 
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

DEF_CMD (jbe, 17, 1, 
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

DEF_CMD (je,  18, 1, 
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

DEF_CMD (jne, 19, 1, 
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
DEF_CMD (jt,  20, 1, 
                    {
                    
                    })

DEF_CMD (call, 21, 1,
                    { 
                    StackPush (processor->calls , processor->pc + sizeof(char) + sizeof(int));
                    processor->pc = *(int*)(processor->code + processor->pc + 1);
                    })

DEF_CMD (ret,  22, 0, 
                    {
                    processor->pc = StackPop(processor->calls);
                    })

DEF_CMD (err, 23, 0,
                    {
                    printf("Discriminant < 0\n");
                    processor->pc += sizeof(char);
                    })

DEF_CMD (noroots, 24, 0,
                    {
                    printf("No roots\n");
                    processor->pc += sizeof(char);
                    })

DEF_CMD (infroots, 25, 0,
                    {
                    printf("INF roots\n");
                    processor->pc += sizeof(char);
                    })
//!!!!
// If add something, don't forget to update NUM_COMANDS constant
//!!!!
