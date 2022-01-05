#include "drag.h"

#include <float.h>

DragState *drag_create()
{
    DragState *drag = (DragState*) malloc(sizeof(DragState));
    if (drag == NULL)
    {
        SDL_Log("Failed to malloc DragState\n");
        return NULL;
    }

    drag->x = 0;
    drag->y = 0;
    drag->vx = 0;
    drag->vy = 0;

    drag->friction = 0.2;
    drag->acceleration = 0.2;

    drag->last_update_time = 0;
    drag->last_input_time = 0;

    drag->last_input_x = 0;
    drag->last_input_y = 0;

    drag->min_x = FLT_MIN;
    drag->max_x = FLT_MAX;
    drag->min_y = FLT_MIN;
    drag->max_y = FLT_MAX;
    drag->snap_spacing_x = 0;
    drag->snap_spacing_y = 0;

    return drag;
}

void drag_free(DragState *drag)
{
    free(drag);
    drag = NULL;
}

void drag_set_limits
(
    DragState *drag,
    float min_x,
    float max_x,
    float min_y,
    float max_y
) {
    drag->min_x = min_x;
    drag->max_x = max_x;
    drag->min_y = min_y;
    drag->max_y = max_y;
}

void drag_set_snapping (
    DragState *drag,
    float snap_spacing_x,
    float snap_spacing_y
) {
    drag->snap_spacing_x = snap_spacing_x;
    drag->snap_spacing_y = snap_spacing_y;
}

/// Get position as SDL_Point
void drag_get_point(DragState *drag, SDL_Point *point)
{
    point->x = drag->x;
    point->y = drag->y;
}

void drag_update(DragState *drag, Uint32 time)
{

    if (drag->last_update_time == 0)
    {
        // Happens only on first update
        drag->last_update_time = time;
    }

    if (drag->free)
    {
        // Update x according to velocity
        drag->x += drag->vx * (float) (time - drag->last_update_time);
        drag->y += drag->vy * (float) (time - drag->last_update_time);

        // Decrease velocity because of friction
        drag->vx *= 1.0 - drag->friction;
        drag->vy *= 1.0 - drag->friction;
    }

    drag->last_update_time = time;
}

void drag_move(DragState *drag, Input *input)
{
    float input_x = (float) input->point.x;
    float input_y = (float) input->point.y;
    Uint32 input_time = input->timestamp;

    drag->x = input_x;
    drag->y = input_y;

    // Ignore if there are 2 inputs with same timestamp
    if (input_time != drag->last_input_time)
    {
        // Ignore first input of drag
        if (drag->last_input_time != 0)
        {
            // Velocity calculated from last two inputs
            float new_vx = (input_x - drag->last_input_x)
                / (float) (input_time - drag->last_input_time);
            float new_vy = (input_y - drag->last_input_y)
                / (float) (input_time - drag->last_input_time);

            // Instead of setting new velocity, we merge it with the previous
            drag->vx = drag->vx * (1.0 - drag->acceleration) + new_vx * drag->acceleration;
            drag->vy = drag->vy * (1.0 - drag->acceleration) + new_vy * drag->acceleration;

            SDL_Log("vx: %f, xy: %f, t: %d, lt: %d, %f",
                        drag->vx,
                        drag->vy,
                        input_time,
                        drag->last_input_time,
                        (float) (input_time - drag->last_input_time)
                    );
        }

        drag->free = false;
        drag->last_input_x = input_x;
        drag->last_input_y = input_y;
        drag->last_input_time = input_time;
    }
}

void drag_up(DragState *drag, Input *input)
{
    drag_move(drag, input);

    drag->last_input_time -= 0;
    drag->free = true;
}
