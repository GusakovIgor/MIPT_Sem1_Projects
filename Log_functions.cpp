#include "MyStack.h"


//-------------------------------------------------------------------------------------------------------
//! LogsClear function:
//!     1) It opens file "Logs.txt" in mode "w" (writing), so file automatically clears
//!     2) Then it closes "Logs.txt"
//!     3) And we can enjoy that we have such good, clean logs file
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void LogsClear ()
{
  FILE* Logs = fopen ("Logs.txt", "w");
  fclose (Logs);
}


//-------------------------------------------------------------------------------------------------------
//! StackOK function:
//!     1) Checks stk on following errors:
//!        a) CANARY_ARRAY_PROTECTION_ERROR
//!        b) CONSTRUCTION_ERROR 
//!        c) MEMORY_ERROR
//!        d) SIZE_ERROR
//!        e) CHECK_IF_EMPTY_ERROR
//!        f) FILLING_ERROR
//!        g) ALL_IS_OK
//!
//!
//! @param [out] stk - stack
//!
//! @return integer - code of the error
//-------------------------------------------------------------------------------------------------------
int StackOK (MyStack* stk)
{
    if (HashChecker (stk) == false)
    {
        return HASH_PROTECTION_ERROR;
    }
    
    if (*CANARY_2L != 0XBEEF || *CANARY_2R != 0XBEEF)
    {
        return CANARY_STACK_PROTECTION_ERROR;
    }
  
    if (*CANARY_1L != 0XC0FFEE || *CANARY_1R != 0XC0FFEE)
    {
        return CANARY_ARRAY_PROTECTION_ERROR;
    }
  
    if (stk == NULL)
    {
        return CONSTRUCTION_ERROR;
    }
  
    if (stk->array == NULL)
    {
        return MEMORY_ERROR;
    }
  
    if (stk->capacity < stk->size || (int*) stk->size < 0)
    {
        return SIZE_ERROR;
    }
  
    if (stk->size == 0 && isnan(stk->array[0]) && stk->is_empty == false)
    {
        return CHECK_IF_EMPTY_ERROR;
    }
  
    if (stk->size < 0)
    {
        return FILLING_ERROR;
    }
  
    for (int i = 0; i < stk->capacity; i++)
    { 
        if ((i <= stk->size && !stk->is_empty && isnan(stk->array[i])) ||
            (i > stk->size  && !isnan(stk->array[i])))
        {
            return FILLING_ERROR;
        }
    }
  
    return I_AM_OK;
}

MyHash* Hash_Construct()
{
    MyHash* hsh = (MyHash*) calloc (1, sizeof(MyHash*));
    assert (hsh != NULL);
    *hsh = {0, 0};
    
    return hsh;
}

void HashMaker (MyStack* stk, const char* check_flag)
{
    int flag = 0;
    if (strcmp(check_flag, "Check") == 0)
    {
        flag = 1;
    }
    else if (strcmp(check_flag, "Default") == 0)
    {
        flag = 0;
    }
    else
    {
        flag = -1;
        printf("Please, enter correct mode for HashMaker function\n");
        assert (flag > 0);
    }
    
    for (char* p = (char*)stk->array; p < (char*)stk->array + stk->capacity*sizeof(double); p++)
    {
        char elem = *p;
        if (flag)
            stk->hsh->value = stk->hsh->value << 1 + ((elem << 3) ^ (elem >> 5)) * elem;
        else
            stk->hsh->temp_value = stk->hsh->temp_value << 1 + ((elem << 3) ^ (elem >> 5)) * elem;
    }
    
    for (char* p = (char*)stk; p < (char*)stk + sizeof(MyStack); p++)
    {
        char elem = *p;
        if (flag)
            stk->hsh->value = stk->hsh->value << 1 + ((elem << 3) ^ (elem >> 5)) * elem;
        else
            stk->hsh->temp_value = stk->hsh->temp_value << 1 + ((elem << 3) ^ (elem >> 5)) * elem;
    }
}


bool HashChecker (MyStack* stk)
{
    HashMaker (stk, "Check");
    if (stk->hsh->temp_value == stk->hsh->value)
        return true;
    else
        return false;
}
