#include "MyStack.h"

int main(int argc, const char* argv[])
{
    LogsClear ();
    
    #ifdef SUPERDEBUG
    if (argc > 1)
    {
      Unit_Tests (argc, argv);
    }
    else
    {
      printf ("Please, enter one of those commands:");
        Help_Print();
      printf ("Or turn SUPERDEBUG mode off");
    }
    
    #else
    size_t capacity = 0;
    scanf ("%u", &capacity);
    assert(capacity >= MIN_CAPACITY);
      
    MyStack* stk = StackConstruct (capacity);
    assert (stk->size == 0);
    assert (stk->capacity == capacity);
        
    /* Breaking stack to show how ASSERT_OK works
    stk->array[3] = 3;
    ASSERT_OK(stk, main)
    */
    
    StackPush(stk, 3);
    StackPush(stk, 4);
    StackPush(stk, 1);
    StackPush(stk, 5);

    StackPop(stk);
    StackPop(stk);
    StackPop(stk);
    StackPush(stk, 9);

    ASSERT_OK(stk, main)

    stk = StackDestruct (stk);
    assert (stk == NULL);
    
    #endif
    
    return 0;
}


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
  if (*CANARY_2L != 0XBEEF || *CANARY_2R != 0XBEEF)
  {
    //printf("Canary_2L = %X\n", *CANARY_2L);
    //printf("Canary_2R = %X\n", *CANARY_2R );
    return CANARY_STACK_PROTECTION_ERROR;
  }
  
  if (*CANARY_1L != 0XC0FFEE || *CANARY_1R != 0XC0FFEE)
  {
    //printf("Canary_1L = %X\n", *CANARY_1L);
    //printf("Canary_1R = %X\n", *CANARY_1R );
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


//-------------------------------------------------------------------------------------------------------
//! StackConstruct function:
//!     1) Creating structure MyStack stk for our stack (in dinamic memory)
//!     2) Filling its fields
//!     3) Creating array (in dinamic memory) and shifting pointer forward on first element (everything before is canary)
//!         a) Array size is bigger than we need on two BYTES for canary protection
//!         b) Filling cannaries we need to make conversions
//!             b_1) double* to char*, otherwise memory distributor will index block of 8 bytes each
//!             b_2) after changing pointer convert it back to double*
//!     4) Fiiling canaries
//!     5) Clearing stack
//!
//! @param [in] capacity - initial capacity of array
//!
//! @return stk - pointer on MyStack structure
//-------------------------------------------------------------------------------------------------------
MyStack* StackConstruct (size_t capacity)
{
    MyStack* stk = (MyStack*) calloc (1*sizeof(MyStack) + 2*CANARY_SIZE, sizeof(char*));
    stk = (MyStack*)((char*)stk + CANARY_SIZE);
    assert (stk != NULL);
    
    *CANARY_2L = 0XBEEF;                  // Making left canary C0FFEE
    *CANARY_2R = 0XBEEF;                  // And right too
    
    *stk = {true, 0, capacity, NULL};
    
    stk->array = (double*) calloc (capacity*sizeof(double) + 2*CANARY_SIZE, sizeof(char));     // Two extra bytes for canaries
    assert (stk->array != NULL);
    stk->array = (double*)((char*)stk->array + CANARY_SIZE);                                   // Shifting pointer on 1 BYTE right (to the array)
    
    *CANARY_1L = 0XC0FFEE;                  // Making left canary C0FFEE
    *CANARY_1R = 0XC0FFEE;                  // And right too

    StackClear (stk);
    
    ASSERT_OK(stk, StackConstruct)
    
    return stk;
}


//-------------------------------------------------------------------------------------------------------
//! StackDestruct function:
//!     1) Deleting arrray using pointer on Canary_1L
//!     2) Deleting stk
//!
//! @param [out] stk - stack
//!
//! @return NULL - means that there's no stack
//-------------------------------------------------------------------------------------------------------
MyStack* StackDestruct (MyStack* stk)
{
    ASSERT_OK(stk, StackDestruct)
    
    free (CANARY_1L);
    free (CANARY_2L);
    
    return NULL;
}


//-------------------------------------------------------------------------------------------------------
//! StackClear function:
//!     1) Making all elements in stack NAN
//!     2) Making stk->size zero (cause we've deleted all elements in stack)
//!     3) And ofcourse making stk->is_empty true
//!
//! @param [out] stk - stack
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void StackClear (MyStack* stk)
{
    if (!stk->is_empty)
    {
      ASSERT_OK(stk, StackClear)
    }
    
    for (int i = 0; i < stk->capacity; i++)
    {
      stk->array[i] = NAN;
    }
    
    stk->size = 0;
    stk->is_empty = true;
    
    ASSERT_OK(stk, StackClear)
}


//-------------------------------------------------------------------------------------------------------
//! StackPush function:
//!     1) Checks that stk is ok
//!     2) If element isn't deepest in stack and not highest, we simply increase size
//!        (and puts in stk->array[stk->size] new element, but it's in paragraph !!!)
//!     3) If element is deepest 
//!     4) And increases stk->size
//!
//! @param [out] stk  - stack
//! @param [in]  elem - value that we need to put in stack
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void StackPush (MyStack* stk, double elem)
{    
    (stk->size == 0 && isnan(stk->array[0])) ? stk->is_empty = true : stk->is_empty = false;
    
    ASSERT_OK(stk, StackPush)
    
    if (stk->is_empty)
    {
      stk->array[stk->size] = elem;
      stk->is_empty = false;
    }
    else
    {
      if (stk->size == stk->capacity - 1)
      {
        StackExpansion(stk);
      }
      stk->size++;
      stk->array[stk->size] = elem;
    }
    
    ASSERT_OK(stk, StackPush)
}


//-------------------------------------------------------------------------------------------------------
//! StackPop function:
//!     1) Checks that stk is ok
//!     3) Saving value of the top element in temporary variable - top
//!     3) Then makes place of this element (in stk) NAN
//!     4) Decreses size only if it's > 0
//!     5) Then if size <= 0.25 * capacity and capacity >= 4:
//!        a) Capacity must be >= 4 cause in other cases new capacity will be 1 or 0,
//!           and that will cause conflicts with size
//!     6) Checks again, that stk is ok
//!
//! @param [out] stk  - stack
//!
//! @return top - value of the first element in stack
//-------------------------------------------------------------------------------------------------------
double StackPop (MyStack* stk)
{
    ASSERT_OK(stk, StackPop)
    
    double top = 0;
    
    if (!stk->is_empty)
    {
      top = stk->array[stk->size];
      stk->array[stk->size] = NAN;
      assert (isnan(stk->array[stk->size]));
      
      if (stk->size == 0)     // ASSERT_OK helped to debug that
      {                       // there was no checking if we pop the last element in stack
        stk->is_empty = true;
      }
    }
    
    if (stk->size > 0) // ASSERT_OK helped to debug that
    {                  // there was no if, and stk->size sometimes was -1
      stk->size--;
    }
    
    if (stk->size <= stk->capacity / 4 && stk->capacity >= 4)
    {
      StackFree(stk);
    }
    
    ASSERT_OK(stk, StackPop)
    
    return top;
}

//-------------------------------------------------------------------------------------------------------
//! StackTop function:
//!     1) Checks that stk is ok
//!     2) And returns value of top element
//!
//! @param [out] stk  - stack
//!
//! @return top - value of the first element in stack
//-------------------------------------------------------------------------------------------------------
double StackTop (MyStack* stk)
{
    ASSERT_OK(stk, StackTop)
    
    return stk->array[stk->size];
}


//-------------------------------------------------------------------------------------------------------
//! StackExpansion function:
//!     1) It checks that stk is ok and that we correctly called this function
//!     2) Doubles memory for the stk->array using realloc
//!     3) It checks again that stk->array isn't NULL (that realloc worked well)
//!     4) Reorganizes memory to use Canaries (just like in StrackConstruct)
//!     5) Then making all new elements NANs
//!     6) And then doubles capacity of stk->array
//!     7) Filling Canaries (only after changing capacity!)
//!     8) Checks again if stk is ok
//!
//! @param [out] stk  - stack
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void StackExpansion (MyStack* stk)
{
    ASSERT_OK(stk, StackExpansion)
    assert (stk->size == stk->capacity - 1);
    
    stk->array = (double*) realloc (CANARY_1L, 2*(stk->capacity)*sizeof(double) + 2*CANARY_SIZE);
    assert (stk->array != NULL);
    stk->array = (double*)((char*)stk->array + CANARY_SIZE);
    
    size_t new_size = 2*stk->capacity;
    for (int i = stk->capacity; i < new_size; i++)
    {
        stk->array[i] = NAN;
    }
    
    stk->capacity = new_size;
    
    *CANARY_1L = 0XC0FFEE;
    *CANARY_1R = 0XC0FFEE;
    
    ASSERT_OK(stk, StackExpansion)
}


//-------------------------------------------------------------------------------------------------------
//! StackFree function:
//!     1) It checks that stk is ok and that we correctly called this function
//!     2) Halves memory for the stk->array using realloc
//!     3) Reorganizes memory to use Canaries (just like in StrackConstruct)
//!     4) And then halves stk->capacity
//!     5) Filling Canaries (only after changing capacity!)
//!     6) Checking again, that stk is ok
//!
//! @param [out] stk  - stack
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void StackFree (MyStack* stk)
{
  ASSERT_OK(stk, StackFree)
  
  assert (stk->size <= stk->capacity/4 && stk->capacity >= 4);
  
  size_t check = stk->capacity;
  
  stk->array = (double*) realloc (CANARY_1L, (stk->capacity)*sizeof(double) / 2 + 2*CANARY_SIZE);
  assert (stk->array != NULL);
  stk->array = (double*)((char*)stk->array + CANARY_SIZE);
  
  stk->capacity = stk->capacity / 2;
  assert (check / 2 == stk->capacity);
  
  *CANARY_1L = 0XC0FFEE;
  *CANARY_1R = 0XC0FFEE;
  
  ASSERT_OK(stk, StackFree)
}
