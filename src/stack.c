#include "stack.h"

#include <stdlib.h>

#include "juan.h"

/// Check if we need to realloc, both to grow and to shrink
static int stack_befunge_realloc(BefungeStack *stack, size_t new_size)
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

BefungeStack *stack_befunge_create()
{
    BefungeStack *stack = (BefungeStack*) malloc(sizeof(BefungeStack));
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

void stack_befunge_free(BefungeStack *stack)
{
    if (stack != NULL)
    {
        free(stack->memory);
        stack->memory = NULL;
    }
    free(stack);
    stack = NULL;
}

int stack_befunge_push(BefungeStack *stack, signed long int v)
{
    if (stack_befunge_realloc(stack, stack->size + 1) != 0)
    {
        SDL_Log("Failed to push to stack\n");
        return 2;
    }

    stack->size += 1;
    stack->memory[stack->size - 1] = v;
    return 0;
}

int stack_befunge_pop(BefungeStack *stack, signed long int *v)
{
    if (stack_befunge_realloc(stack, stack->size - 1) != 0)
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

void stack_befunge_print(BefungeStack *stack)
{
    SDL_Log("size: %lu, alloc_size: %lu\n", stack->size, stack->alloc_size);
    SDL_Log("Bottom of stack... (first item pushed)\n");
    for (size_t i = 0; i < stack->size; i++)
    {
        SDL_Log("%lu\n", stack->memory[i]);
    }
    SDL_Log("Top of stack... (last item pushed)\n");
}
