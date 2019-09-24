#include <cstring>
#include <stdio.h>
#include <assert.h>

#define DEBUG 1

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef int stackElement_t;
stackElement_t INITIALIZE_VALUE = 0;
const int STACK_SIZE = 5;

struct Stack_t {
    stackElement_t data[STACK_SIZE];
    int size;
#ifdef DEBUG
    char *name;
#endif
};

Stack_t StackConstruct ()
{
  Stack_t stack = Stack_t{};
  memset (stack.data, INITIALIZE_VALUE, sizeof (stackElement_t) * STACK_SIZE);

  return stack;
}

void StackDump (Stack_t *stack, int line, FILE *stream = stdout)
{
  fprintf (stream, ANSI_COLOR_BLUE "Stack Dump Line: %d Stack_t \"%s\" [%p] \n{\n" ANSI_COLOR_RESET, line, stack->name, stack);
  for (int i = 0; i < STACK_SIZE; i++)
    {
      if (stack->data[i] != INITIALIZE_VALUE)
        {
          fprintf (stream, ANSI_COLOR_GREEN "[*] %d [%p]\n" ANSI_COLOR_RESET, stack->data[i], &stack->data[i]);
        }
        else
        {
          fprintf (stream, ANSI_COLOR_RED "%d [%p]\n" ANSI_COLOR_RESET, stack->data[i], &stack->data[i]);
        }
    }
  fprintf (stream, ANSI_COLOR_BLUE "}\n\n" ANSI_COLOR_RESET);
}

bool StackPush (Stack_t *stack, stackElement_t value)
{
  assert(stack);

  if (stack->size < STACK_SIZE)
    {
      stack->data[stack->size++] = value;

      return true;
    }

  StackDump (stack, __LINE__);
  return false;
}

bool StackPop (Stack_t *stack, stackElement_t *value)
{
  assert (stack);

  if (stack->size)
    {
      *value = stack->data[stack->size--];
      stack->data[stack->size] = INITIALIZE_VALUE;
      return true;
    }
  return false;
}

bool StackPeek (Stack_t *stack, stackElement_t *value)
{
  assert (stack);

  if (stack->size)
    {
      *value = stack->data[stack->size-1];
      return true;
    }
  return false;;
}

bool StackDestruct (Stack_t *stack)
{
  assert(stack);

  stack->size = 0;
  memset (stack->data, INITIALIZE_VALUE, sizeof (stackElement_t) * STACK_SIZE);

#ifdef DEBUG
  stack->name = 0;
#endif
  return true;
}

int main ()
{
  Stack_t stack = StackConstruct ();
  stack.name = "stack";

  StackPush (&stack, 1);
  StackPush (&stack, 2);

  StackDump (&stack, __LINE__);

  stackElement_t value = 0;
  StackPop (&stack, &value);

  StackDump (&stack, __LINE__);

  StackDestruct (&stack);
  StackDump (&stack, __LINE__);
  return 0;
}