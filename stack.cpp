//
// Created by maxim on 24.09.19.
//

#include "stack.h"

stackElement_t INITIALIZE_VALUE = 0;

const int STACK_SIZE = 1;

void StackDump (Stack_t *stack, int line, FILE *stream)
{
  fprintf (stream, ANSI_COLOR_BLUE "Stack Dump Line: %d Stack_t \"%s\" [%p] \n{\n" ANSI_COLOR_RESET, line, stack->name, stack);
  for (int i = 0; i < stack->size; i++)
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

bool StackOk (Stack_t *stack)
{
  assert (stack);

  for (int i = 0; i < stack->size; i++)
    {
      if (stack->data[i] == INITIALIZE_VALUE)
        {
          return false;
        }
      stack->data++;
    }
  return true;
}

bool StackLogging (Stack_t *stack, int line, FILE *stream, bool silent)
{
  if (!StackOk (stack))
    {
      if (silent)
        {
          StackDump (stack, line, stream);
          return false;
        }
      else
        {
          assert(StackOk (stack));
        }
    }
  return true;
}

Stack_t StackConstruct ()
{
  Stack_t stack;
  stack.data = (stackElement_t *) calloc (1, sizeof (stackElement_t));
  stack.size = 0;
  stack.current_max_size = 1;

  StackLogging (&stack, __LINE__, stdout);

  return stack;
}

bool ReallocateStack (Stack_t *stack, unsigned int size_multiplier)
{
  assert(stack);
  //assert (StackLogging (stack, __LINE__, stdout));

  stack->current_max_size = stack->size * size_multiplier;
  stack->data = (stackElement_t *) realloc (stack->data, stack->current_max_size * sizeof (stackElement_t));

  //assert(StackLogging (stack, __LINE__, stdout));

  return true;
}

bool StackPush (Stack_t *stack, stackElement_t value)
{
  assert(stack);
  //assert(StackLogging (stack, __LINE__, stdout));

  if (stack->size < stack->current_max_size)
    {
      stack->data[stack->size++] = value;
    }
  else
    {
      StackDump (stack, __LINE__);
      ReallocateStack (stack, 2);
      StackDump (stack, __LINE__);
      stack->data[stack->size++] = value;
    }

  //assert(StackLogging (stack, __LINE__, stdout));

  return true;
}

bool StackPop (Stack_t *stack, stackElement_t *value)
{
  assert (stack);
  //assert(StackLogging (stack, __LINE__, stdout));

  if (stack->size)
    {
      *value = stack->data[stack->size--];
      stack->data[stack->size] = INITIALIZE_VALUE;

      //assert(StackLogging (stack, __LINE__, stdout));
      return true;
    }
  return false;
}

bool StackPeek (Stack_t *stack, stackElement_t *value)
{
  assert (stack);
  assert(StackLogging (stack, __LINE__, stdout));

  if (stack->size)
    {
      *value = stack->data[stack->size - 1];
      return true;
    }
  return false;;
}

bool StackDestruct (Stack_t *stack)
{
  assert(stack);
  //assert(StackLogging (stack, __LINE__, stdout));

  stack->size = 0;
  memset (stack->data, INITIALIZE_VALUE, sizeof (stackElement_t) * STACK_SIZE);

#ifdef DEBUG
  stack->name = 0;
#endif
  return true;
}
