#ifndef INTRPR_H
#define INTRPR_H

#include "canvas.h"
#include "stack.h"

enum INTRPR_STATE
{
    INTRPR_PAUSED,
    INTRPR_RUNNING,
    INTRPR_STATE_TOTAL,
};

/// Fungeoid interpreter
/**
 * Contains the state and functionality related to interpreting the code. But
 * also is responsible to run at the requested speed, start, stop, etc.
 *
 * Contains the canvas, stack, instruction pointer and defines the instructions.
 */
typedef struct Intrpr
{
    // Absolute time in milliseconds when last simulation step was done
    Uint32 _last_step_ms;
    // Time interval between steps
    Uint32 _interval_step_ms;

    // If running or not
    enum INTRPR_STATE _state;

    SDL_Point _ip; // Instruction pointer position
    SDL_Point _ip_sp; // IP Speed. Next relative motion of IP, e.g. (-1, 0)

    Stack *_stack;
    Canvas *_canvas;
} Intrpr;

/// Create interpreter
/**
 * Must be freed.
 *
 * On error returns NULL.
 */
Intrpr *intrpr_create();

void intrpr_free(Intrpr *intrpr);

/// Get reference to canvas, it is owned by intrpr so it should not be freed
Canvas *intrpr_get_canvas(Intrpr* intrpr);

/// Get reference to stack, it is owned by intrpr so it should not be freed
Stack *intrpr_get_stack(Intrpr* intrpr);

SDL_Point intrpr_get_ip(Intrpr* intrpr);

SDL_Point intrpr_get_ip_speed(Intrpr* intrpr);

void intrpr_move_ip(Intrpr* intrpr, int x, int y);

void intrpr_move_ip_rel(Intrpr* intrpr, int x, int y);

enum INTRPR_STATE intrpr_get_state(Intrpr* intrpr);

/// Resets stack and IP
void intrpr_reset(Intrpr* intrpr);

/// Starts or continues running. Does not reset state,
void intrpr_resume(Intrpr* intrpr);

/// Starts or continues running. Does not reset state,
void intrpr_pause(Intrpr* intrpr);

/// Run simulation step
/**
 * Executes instruction where IP is and then moves the IP.
 */
void intrpr_step(Intrpr *intrpr);

/// Increase execution speed
void intrpr_faster(Intrpr *intrpr);

/// Decrease execution speed
void intrpr_slower(Intrpr *intrpr);

/// Run simulation step if the time passed is enough
void intrpr_update(Intrpr *intrpr, Uint32 time_abs_ms);

#endif
