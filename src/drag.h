#ifndef DRAG_H
#define DRAG_H

#include <stdbool.h>

#include "juan.h"
#include "input.h"

/// Tools related to the inertia, friction, etc. of dragging, swiping, etc.

/// Keeps track of the movement of something you can drag or swipe
/**
 * Consider the members as private.
 *
 * Receives touch/mouse input events and calculates the movement of something
 * that was dragged, for example when scrolling the field or when dragging
 * horizontally the keyboard to switch tabs.
 *
 * Apart from enabling some kind of swiping where the thing continues moving
 * with some inertia when the finger was raised, you can set snap spacing so the
 * thing tends to move to the closest snap point. For example if snap_spacing_x
 * is equal to 100, then the x position will move until it rests on 0, 100, 200,
 * etc. as X coordinate.
 *
 * You can also set minimum and maximum limits for coordinates.
 */
typedef struct DragState
{
    float x;
    float y;
    float vx;
    float vy;

    // Number less than 1 indicating how much to decrease velocity
    float friction;
    // Number less than 1 indicating how much to follow new inputs velocity
    float acceleration;

    /// True if the finger was released, false if the thing is being dragged
    bool free;

    // Not the time of last event. is the time of the last update, that is the
    // last frame
    Uint32 last_update_time;
    // Time of last input event
    Uint32 last_input_time;

    // Position of last input
    float last_input_x;
    float last_input_y;

    // These should be set to FLT_MAX or FLT_MIN if not used
    float min_x;
    float max_x;
    float min_y;
    float max_y;

    // These should be set to 0 if not used
    float snap_spacing_x;
    float snap_spacing_y;

} DragState;

/// Create DragState
/**
 * Must be freed. On error returns NULL.
 *
 * By default, there is no snapping and no limits to coordinates enabled, you
 * can set them afterwards using the appropiate function.
 */
DragState *drag_create();

/// Free DragState memory
void drag_free(DragState *drag);

/// Set limits to coordinates
/**
 * If you want to disable a limit on some coordinate, set the value to FLT_MIN
 * or FLT_MAX accordingly.
 */
void drag_set_limits
(
    DragState *drag,
    float min_x,
    float max_x,
    float min_y,
    float max_y
);

/// Set snapping
/**
 * If you want to disable snapping on some dimension set the value to zero.
 */
void drag_set_snapping (
    DragState *drag,
    float snap_spacing_x,
    float snap_spacing_y
);

/// Get position as SDL_Point
void drag_get_point(DragState *drag, SDL_Point *point);

/// Update
/**
 * Should be called repeatedly once per frame, it doesn't matter if the user is
 * touching or not.
 */
void drag_update(DragState *drag, Uint32 time);

/// Update from a move input event
/**
 * Should be called only if the event is INPUT_CLICK_MOVE.
 * If the event is INPUT_CLICK_MOVE_UP you should call drag_up().
 */
void drag_move(DragState *drag, Input *input);

/// Update from a finger release input event
/**
 * Should be called only if the event is INPUT_CLICK_MOVE_UP.
 */
void drag_up(DragState *drag, Input *input);

#endif
