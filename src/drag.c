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

    memset(drag->x_history, 0, DRAG_HISTORY * sizeof(*drag->x_history));
    memset(drag->y_history, 0, DRAG_HISTORY * sizeof(*drag->y_history));
    memset(drag->t_history, 0, DRAG_HISTORY * sizeof(*drag->t_history));

    drag->last_update_time = 0;

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
        /*
         * Current X = Prev X + VX * (time - last_update_time)
         *
         * VX = VX * 0.7
         * if (VX < 0.4) { VX = 0 }
         */
        /* drag->vx = 0.05; */
        /* drag->vy = 0.05; */
        drag->x += drag->vx * (float) (time - drag->last_update_time);
        drag->y += drag->vy * (float) (time - drag->last_update_time);

        drag->vx *= 0.8;
        drag->vy *= 0.8;

        memset(drag->x_history, 0, DRAG_HISTORY * sizeof(*drag->x_history));
        memset(drag->y_history, 0, DRAG_HISTORY * sizeof(*drag->y_history));
        memset(drag->t_history, 0, DRAG_HISTORY * sizeof(*drag->t_history));

        /* if (drag->vx < 0.05) */
        /* { */
            /* drag->vx = 0; */
        /* } */
        /* if (drag->vy < 0.05) */
        /* { */
            /* drag->vy = 0; */
        /* } */
        /* SDL_Log("%d, %d", time, drag->last_update_time); */
        /* SDL_Log("pos: [%f,%f], vel: [%e,%e] FREE", drag->x, drag->y, drag->vx, drag->vy); */
    }
    else
    {
        /* for (int i = 0; i < DRAG_HISTORY; i++) */
        /* { */
            /* SDL_Log("%d", drag->t_history[i]); */
        /* } */
        SDL_Log("-----------");
        /*
         * Current X = X[-1]
         *
         * VX = (X[-1] - X[-2])/(time - T[-2])
         */
        if (time != drag->t_history[1])
        {
            drag->x = drag->x_history[0];
            drag->y = drag->y_history[0];

            drag->vx = 0;
            drag->vx += (drag->x_history[0] - drag->x_history[1])
                / (float) (time - drag->t_history[1]) / 3;
            drag->vx += (drag->x_history[0] - drag->x_history[2])
                / (float) (time - drag->t_history[2]) / 3;
            drag->vx += (drag->x_history[0] - drag->x_history[3])
                / (float) (time - drag->t_history[3]) / 3;

            drag->vy = 0;
            SDL_Log("vy: %f, y: %f", drag->vy, 0.);

            drag->vy += (drag->y_history[0] - drag->y_history[1])
                / (float) (time - drag->t_history[1]) / 3;
            SDL_Log("vy: %f, y: %f, t: %d",
                drag->vy,
                drag->y_history[0] - drag->y_history[1],
                drag->t_history[0] - drag->t_history[1]
            );

            drag->vy += (drag->y_history[0] - drag->y_history[2])
                / (float) (time - drag->t_history[2]) / 3;
            SDL_Log("vy: %f, y: %f, t: %d",
                drag->vy,
                drag->y_history[0] - drag->y_history[2],
                drag->t_history[0] - drag->t_history[2]
            );

            drag->vy += (drag->y_history[0] - drag->y_history[3])
                / (float) (time - drag->t_history[3]) / 3;
            SDL_Log("vy: %f, y: %f, t: %d",
                drag->vy,
                drag->y_history[0] - drag->y_history[3],
                drag->t_history[0] - drag->t_history[3]
            );

            /* SDL_Log("%d, %d", time, drag->t_history[1]); */
            /* SDL_Log("pos: [%f,%f], vel: [%e,%e]", drag->x, drag->y, drag->vx, drag->vy); */
        }
        else
        {
            return;
        }
    }

    drag->last_update_time = time;
}

void drag_move(DragState *drag, Input *input)
{
    if (input->timestamp == drag->t_history[0])
    {
        return;
    }
    // Move every value of the history one index forward (discarding the last)
    for (int i = DRAG_HISTORY - 2; i >= 0; i--)
    {
        drag->x_history[i + 1] = drag->x_history[i];
        drag->y_history[i + 1] = drag->y_history[i];
        drag->t_history[i + 1] = drag->t_history[i];
    }

    // Set current value
    drag->x_history[0] = (float) input->point.x;
    drag->y_history[0] = (float) input->point.y;
    drag->t_history[0] = (float) input->timestamp;

    drag->free = false;
}

void drag_up(DragState *drag, Input *input)
{
    drag_move(drag, input);

    drag->free = true;
}
