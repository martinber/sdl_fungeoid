#ifndef STACK_H
#define STACK_H

#include <stddef.h>

/// Stack of signed long int numbers
/**
 * Grows dynamically when needed, Consider the member variables private
 * excluding alloc_size.
 */
typedef struct Stack
{
    // Maximum amount of numbers (not bytes) to hold, realloc() will be called
    // only if this maximum was not reached
    // Each number has a size of number_size
    size_t max_size;

    // Maximum amount of numbers (not bytes) that can fit in the current memory
    // allocation, more than that and realloc() will be called
    // Each number has a size of number_size
    size_t alloc_size;

    // When realloc(), how much to increment alloc_size (in numbers, not bytes).
    // Also, if size < alloc_size - alloc_step, realloc() is called to make
    // allocated memory smaller.
    size_t alloc_step;

    // Current stack size (in numbers, not bytes)
    size_t size;

    // Size of each element, equal to sizeof(unsigned long int)
    size_t number_size;

    // Pointer to memory
    signed long int *memory;
} Stack;

/*
/// Stack of stacks
typedef struct StackStack
{
    // Maximum amount of stacks to hold, realloc() will be called only if this
    // maximum was not reached
    // Each number has a size of sizeof(signed long int)
    size_t max_size;

    // Maximum amount of numbers that can fit in the current memory allocation,
    // more than that and realloc() will be called
    // Each number has a size of sizeof(signed long int)
    size_t alloc_size;

    // Current stack size
    size_t size;

    // Pointer to memory
    signed long int *memory;
} StackStack;
*/

/// Create stack
/**
 * Must be freed.
 *
 * On error returns NULL.
 */
Stack *stack_create();

/// Free stack memory
void stack_free(Stack *stack);

/// Push value to stack
/**
 * Returns 2 if failed to allocate memory, remember to later free the stack
 * anyways
 */
int stack_push(Stack *stack, signed long int v);

/// Pop value from stack
/**
 * Returns 1 if stack was empty
 * Returns 2 if failed to allocate memory, remember to later free the stack
 * anyways
 */
int stack_pop(Stack *stack, signed long int *v);

/// Clear stack
/**
 * Returns 2 if failed to allocate memory, remember to later free the stack
 * anyways
 */
int stack_clear(Stack *stack);

/// Print stack to SDL_Log for debugging
void stack_print(Stack *stack);

#endif
