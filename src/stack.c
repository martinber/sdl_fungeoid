#include "stack.h"

#include <stdlib.h>

#include "juan.h"

/// Check if we need to realloc, both to grow and to shrink
static int stack_realloc(Stack *stack, size_t new_size)
{
    if (
        new_size > stack->alloc_size
        || new_size < stack->alloc_size - stack->alloc_step
    ) {
        // Find new alloc size, should be the smallest multiple of alloc_step
        // that can hold the new_size
        size_t new_alloc_size =
                ((new_size / stack->alloc_step) + 1) * stack->alloc_step;

        signed long int *new_memory = (signed long int*)
                realloc(stack->memory, new_alloc_size * stack->number_size);
        if (new_memory == NULL)
        {
            SDL_Log("Failed to realloc stack\n");
            return 1;
        }
        else
        {
            stack->memory = new_memory;
            stack->alloc_size = new_alloc_size;
        }
    }
    return 0;
}

Stack *stack_create()
{
    Stack *stack = (Stack*) malloc(sizeof(Stack));
    if (stack == NULL)
    {
        SDL_Log("Failed to malloc stack\n");
        return NULL;
    }
    stack->max_size = 100000;
    stack->alloc_size = 0;
    stack->alloc_step = 1000;
    stack->size = 0;
    stack->number_size = sizeof(unsigned long int);
    stack->memory = NULL;

    stack->memory = (signed long int*) malloc(stack->alloc_step * stack->number_size);
    if (stack->memory == NULL)
    {
        free(stack);
        stack = NULL;
        SDL_Log("Failed to malloc stack->memory\n");
        return NULL;
    }

    return stack;
}

void stack_free(Stack *stack)
{
    if (stack != NULL)
    {
        free(stack->memory);
        stack->memory = NULL;
    }
    free(stack);
    stack = NULL;
}

int stack_push(Stack *stack, signed long int v)
{
    if (stack_realloc(stack, stack->size + 1) != 0)
    {
        SDL_Log("Failed to push to stack\n");
        return 2;
    }

    stack->size += 1;
    stack->memory[stack->size - 1] = v;
    return 0;
}

int stack_pop(Stack *stack, signed long int *v)
{
    if (stack_realloc(stack, stack->size - 1) != 0)
    {
        SDL_Log("Failed to pull from stack\n");
        return 2;
    }
    else if (stack->size == 0)
    {
        SDL_Log("Tried to pull from empty stack\n");
        return 1;
    }

    *v = stack->memory[stack->size - 1];
    stack->size -= 1;
    return 0;
}

int stack_clear(Stack *stack)
{
    if (stack_realloc(stack, 0) != 0)
    {
        SDL_Log("Failed to clear and reallocate stack\n");
        return 2;
    }

    stack->size = 0;
    return 0;
}

void stack_print(Stack *stack)
{
    SDL_Log("size: %zu, alloc_size: %zu\n", stack->size, stack->alloc_size);
    SDL_Log("Bottom of stack... (first item pushed)\n");
    for (size_t i = 0; i < stack->size; i++)
    {
        SDL_Log("%lu\n", stack->memory[i]);
    }
    SDL_Log("Top of stack... (last item pushed)\n");
}
