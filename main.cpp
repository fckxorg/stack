#include "stack.h"

int main ()
{
  Stack_t stack = StackConstruct ();
  stack.name = "stack";

  StackPush (&stack, 1);
  StackPush (&stack, 2);

  StackDump (&stack, __FILE__, __LINE__);

  stackElement_t value = 0;
  StackPop (&stack, &value);

  StackDump (&stack, __FILE__, __LINE__);
  StackDestruct (&stack);
  return 0;
}