//
// Created by maxim on 24.09.19.
//
#define STACK_OK assert (StackLogging(stack, __FILE__, __LINE__, stdout))

#include "stack.h"

stackElement_t INITIALIZE_VALUE = 0;

const int STACK_SIZE = 1;
const float low_multiplier = 0.7;

void StackDump (Stack_t *stack, const char *file, const int line, FILE *stream)
{
  /*! Writes log with debug data to provided file or stdout
   * @param stack pointer to stack structure
   * @param file string with filename
   * @param line number of line
   * @param stream file for output (stdin by default)
   * */

  fprintf (stream, ANSI_COLOR_BLUE "*Stack Dump* %s  %s: line: %d Stack_t \"%s\" [%p] \n{\n" ANSI_COLOR_RESET, __DATE__, file, line, stack->name, stack);
  for (size_t i = 0; i < stack->size; i++)
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

  /*! Stack verificator
   * @param stack pointer to stack
   * @return state true or false if stack valid or not respectively
   * */

  for (size_t i = 0; i < stack->size; i++)
    {
      if (stack->data[i] == INITIALIZE_VALUE)
        {
          return false;
        }
    }
  return true;
}

bool StackLogging (Stack_t *stack, const char *file, int line, FILE *stream, bool silent)
{
  /*! Function wrapper for logging
   * @param stack ptr to stack
   * @param file string with name of file
   * @param line number of line
   * @param stream file for output (stdout by default)
   * @param silent flag to specify if logger should use asserts or log everything silently
   * @return state true or false if stack is valid or not respectively
   * */

  if (!StackOk (stack))
    {
      if (silent)
        {
          StackDump (stack, file, line, stream);
          return false;
        }
      else
        {
          assert(StackOk (stack));
        }
    }
  return true;
}

Stack_t StackConstruct (size_t size)
{
  /*! Stack constructor
   * Initializes base structure
   * @param size defines start size of stack
   * @return stack returns stack structure
   * */

  auto stack = Stack_t{};
  stack.data = (stackElement_t *) calloc (size, sizeof (stackElement_t));
  stack.size = 0;
  stack.current_max_size = size;

  return stack;
}

bool ReallocateStack (Stack_t *stack, float size_multiplier)
{
  /*! Expands or folds stack
   * @param stack ptr to stack
   * @param size_multiplier
   * @return state true or false if stack was successfully expanded or not respectively
   * */

  assert(stack);

  stack->current_max_size = (size_t)(stack->size * size_multiplier);
  auto tmp = (stackElement_t *) realloc (stack->data, stack->current_max_size * sizeof (stackElement_t));

  if (!tmp)
    {
      return false;
    }
  stack->data = tmp;

  STACK_OK;

  return true;
}

bool StackPush (Stack_t *stack, stackElement_t value)
{
  /*! Adds new element to stack
   * @param stack ptr to stack
   * @param value value to add
   * */

  assert(stack);

  STACK_OK;

  if (stack->size < stack->current_max_size)
    {
      stack->data[stack->size++] = value;
    }
  else
    {
      ReallocateStack (stack, 2);
      stack->data[stack->size++] = value;
    }

  STACK_OK;

  return true;
}

bool StackPop (Stack_t *stack, stackElement_t *value)
{
  /*! Removes last element from stack and saves it to provided variable
   * @param stack pointer to stack
   * @param value pointer to variable for value
   * @return true or false if pop was successful or not respectively
   * */

  assert (stack);
  STACK_OK;

  if (stack->size)
    {
      *value = stack->data[stack->size--];
      stack->data[stack->size] = INITIALIZE_VALUE;

      STACK_OK;

      return true;
    }

  if (stack->size < stack->current_max_size * low_multiplier)
    {
      ReallocateStack (stack, low_multiplier);
    }

    STACK_OK;

  return false;
}

bool StackPeek (Stack_t *stack, stackElement_t *value)
{
  /*! Saves last element from stack to provided variable
   * @param stack pointer to stack
   * @param value pointer to variable for value
   * @return true or false if pop was successful or not respectively
   * */

  assert (stack);
  STACK_OK;

  if (stack->size)
    {
      *value = stack->data[stack->size - 1];
      return true;
    }

  return false;
}

bool StackDestruct (Stack_t *stack)
{
  /*! Destructs stack
   * @param stack pointer to stack
   * @return state true or false if stack destruction was successful or not
   * */
  assert(stack);
  STACK_OK;

  stack->size = NULL;
  free (stack->data);

#ifdef DEBUG
  stack->name = nullptr;
#endif

  return true;
}
