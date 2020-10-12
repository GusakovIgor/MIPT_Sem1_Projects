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
    printf("%u", sizeof(long long int));
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
