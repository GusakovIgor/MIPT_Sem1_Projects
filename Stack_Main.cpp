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
        
    // Breaking stack to show how ASSERT_OK works
    stk->array[3] = 3;
    ASSERT_OK(stk)
    //
        
    StackPush(stk, 3);
    StackPush(stk, 4);
    StackPush(stk, 1);
    StackPush(stk, 5);

    StackPop(stk);
    StackPop(stk);
    StackPop(stk);
    StackPush(stk, 9);
        
    /*
    StackDump(stk)
    */
        
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
//!        a) CONSTRUCTION_ERROR 
//!        b) MEMORY_ERROR
//!        c) SIZE_ERROR
//!        d) CHECK_IF_EMPTY_ERROR
//!        e) FILLING_ERROR
//!        f) ALL_IS_OK
//!
//!
//! @param [out] stk - stack
//!
//! @return integer - code of the error
//-------------------------------------------------------------------------------------------------------
int StackOK (MyStack* stk)
{
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
//!     2) Filling its capacity
//!     3) Creating array (in dinamic memory) and changes corresponding variable in stk
//!
//! @param [in] capacity - initial capacity of array
//!
//! @return stk - pointer on MyStack structure
//-------------------------------------------------------------------------------------------------------
MyStack* StackConstruct (size_t capacity)
{
    MyStack* stk = (MyStack*) calloc (1, sizeof(MyStack));
    assert (stk != NULL);
    
    *stk = {true, 0, capacity, NULL};
    
    stk->array = (double*) calloc (capacity, sizeof(double));
    assert (stk->array != NULL);
    
    StackClear (stk);
    
    ASSERT_OK(stk)
    
    return stk;
}


//-------------------------------------------------------------------------------------------------------
//! StackDestruct function:
//!     1) Deleting stk->array
//!     2) Deleting stk
//!
//! @param [out] stk - stack
//!
//! @return NULL - means that there's no stack
//-------------------------------------------------------------------------------------------------------
MyStack* StackDestruct (MyStack* stk)
{
    ASSERT_OK(stk)
    
    free (stk->array);
    free (stk);
    
    return NULL;
}


//-------------------------------------------------------------------------------------------------------
//! StackClear function:
//!     1) Making all elements in stack NAN
//!     2) Making stk->size zero (cause we've deleted all elements in stack)
//!
//! @param [out] stk - stack
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void StackClear (MyStack* stk)
{
    if (!stk->is_empty)
    {
      ASSERT_OK(stk)
    }
    
    for (int i = 0; i < stk->capacity; i++)
    {
        stk->array[i] = NAN;
    }
    
    stk->size = 0;
    
    ASSERT_OK(stk)
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
    
    ASSERT_OK(stk)
    
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
    
    ASSERT_OK(stk)
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
    ASSERT_OK(stk)
    
    double top = 0;
    
    if (!stk->is_empty)
    {
      top = stk->array[stk->size];
      stk->array[stk->size] = NAN;
      assert (isnan(stk->array[stk->size]));
    }
    
    
    if (stk->size > 0) // ASSERT_OK helped to debug that
    {                  // there was no if, and stk->size sometimes was -1
      stk->size--;
    }
    
    if (stk->size <= 0.25 * stk->capacity && stk->capacity >= 4)
    {
      StackFree(stk);
    }
    
    ASSERT_OK(stk)
    
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
    ASSERT_OK(stk)
    
    return stk->array[stk->size];
}


//-------------------------------------------------------------------------------------------------------
//! StackExpansion function:
//!     1) It checks that stk is ok and that we correctly called this function
//!     2) Doubles memory for the stk->array using realloc
//!     3) It checks again that stk->array isn't NULL (that realloc worked well)
//!     4) Then making all new elements NANs
//!     5) And then doubles capacity of stk->array
//!     6) Checks again if stk is ok
//!
//! @param [out] stk  - stack
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void StackExpansion (MyStack* stk)
{
    ASSERT_OK(stk)
    assert (stk->size == stk->capacity - 1);
    
    stk->array = (double*) realloc (stk->array, 2*(stk->capacity)*sizeof(double));
    assert (stk->array != NULL);
    
    size_t new_size = 2*stk->capacity;
    for (int i = stk->capacity; i < new_size; i++)
    {
        stk->array[i] = NAN;
    }
    
    stk->capacity = new_size;
    
    ASSERT_OK(stk)
}


//-------------------------------------------------------------------------------------------------------
//! StackFree function:
//!     1) It checks that stk is ok and that we correctly called this function
//!     2) Halves memory for the stk->array using realloc
//!     3) And then halves stk->capacity
//!     4) Checking again, that stk is ok
//!
//! @param [out] stk  - stack
//!
//! @return void
//-------------------------------------------------------------------------------------------------------
void StackFree (MyStack* stk)
{
  ASSERT_OK(stk)
  assert (stk->size <= stk->capacity/4 && stk->capacity >= 4);
  
  size_t check = stk->capacity;
  
  stk->array = (double*) realloc (stk->array, (stk->capacity)*sizeof(double) / 2);
  stk->capacity = stk->capacity / 2;
  
  assert (check / 2 == stk->capacity);
  
  ASSERT_OK(stk)
}
