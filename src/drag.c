#include "drag.h"

#include <float.h>
#include <math.h>

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

    drag->friction = 0.1;
    drag->input_accel = 0.2;
    drag->snap_accel = 0.005;

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

/// Modulo that is always positive
/**
 * https://stackoverflow.com/a/52529440
 */
float euclidean_modulo(float a, float b) {
    float m = fmodf(a, b);
    if (m < 0) {
        m = (b < 0) ? m - b : m + b;
    }
    return m;
}

/// Update in one coordinate
/**
 * That is will calculate position, speed, snapping in x or y depending in
 * arguments. It will modify arguments in place.
 */
static void update_coord
(
    float *pos, float *vel, float snap_spacing, float dt,
    float friction, float snap_accel
) {

    // Update position according to velocity
    *pos += (*vel) * dt;

    // Add slight velocity to closest snap point
    if (snap_spacing != 0)
    {
        // Always negative
        float prev_snap_dist = - euclidean_modulo(*pos, snap_spacing);
        // Always positive
        float next_snap_dist = prev_snap_dist + snap_spacing;

        if (fabs(prev_snap_dist) < fabs(next_snap_dist)) {
            *vel += prev_snap_dist * snap_accel;
        }
        else
        {
            *vel += next_snap_dist * snap_accel;
        }

        if (next_snap_dist < 1 && fabs(*vel) < 0.1)
        {
            *pos += next_snap_dist;
            *vel = 0;
        }
    }

    // Decrease velocity because of friction
    *vel *= (1.0 - friction);
    *vel *= (1.0 - friction);
}

void drag_update(DragState *drag, Uint32 time)
{
    float delta_time = (float) (time - drag->last_update_time);

    if (drag->last_update_time == 0)
    {
        // Happens only on first update
        drag->last_update_time = time;
    }

    if (drag->free)
    {
        update_coord(&(drag->x), &(drag->vx), drag->snap_spacing_x, delta_time,
                drag->friction, drag->snap_accel);
        update_coord(&(drag->y), &(drag->vy), drag->snap_spacing_y, delta_time,
                drag->friction, drag->snap_accel);
    }

    drag->last_update_time = time;
}

void drag_move(DragState *drag, SDL_Point point, Uint32 timestamp)
{
    float input_x = (float) point.x;
    float input_y = (float) point.y;
    Uint32 input_time = timestamp;

    // Ignore if there are 2 inputs with same timestamp
    if (input_time != drag->last_input_time)
    {
        // Ignore first input of drag
        if (drag->last_input_time != 0)
        {
            drag->x += input_x - drag->last_input_x;
            drag->y += input_y - drag->last_input_y;

            // Velocity calculated from last two inputs
            float new_vx = (input_x - drag->last_input_x)
                / (float) (input_time - drag->last_input_time);
            float new_vy = (input_y - drag->last_input_y)
                / (float) (input_time - drag->last_input_time);

            // Instead of setting new velocity, we merge it with the previous
            drag->vx = drag->vx * (1.0 - drag->input_accel) + new_vx * drag->input_accel;
            drag->vy = drag->vy * (1.0 - drag->input_accel) + new_vy * drag->input_accel;
        }

        drag->free = false;
        drag->last_input_x = input_x;
        drag->last_input_y = input_y;
        drag->last_input_time = input_time;
    }
}

void drag_up(DragState *drag)
{
    drag->last_input_time = 0;
    drag->free = true;
}
