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

    memset(drag->x_history, 0, DRAG_HISTORY);
    memset(drag->y_history, 0, DRAG_HISTORY);
    memset(drag->t_history, 0, DRAG_HISTORY);

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

}

void drag_move(DragState *drag, Input *input)
{

}

void drag_up(DragState *drag, Input *input)
{

}

#endif
