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

DEF_CMD (HLT,  0, 0, 
                    {
                        
                    })

DEF_CMD (IN,   1, 0, 
                    {
                    int number = 0;
                    scanf ("%d", &number);
                    StackPush(&stk, number);
                    })

DEF_CMD (OUT,  2, 0, 
                    {
                    printf ("%d", StackPop(&stk));
                    })

DEF_CMD (PUSH, 3, 2, 
                    {
                    
                    })

DEF_CMD (POP,  4, 2, 
                    {
                    
                    })

DEF_CMD (ADD,  5, 0, 
                    {
                    
                    })

DEF_CMD (SUB,  6, 0, 
                    {
                    
                    })

DEF_CMD (MULT, 7, 0, 
                    {
                    
                    })

DEF_CMD (SEGM, 8, 0, 
                    {
                    
                    })

DEF_CMD (POW,  9, 0, 
                    {
                    
                    })

DEF_CMD (SQRT, 10, 0, 
                    {
                    
                    })

DEF_CMD (SIN, 11, 0, 
                    {
                    
                    })

DEF_CMD (COS, 12, 0, 
                    {
                    
                    })
