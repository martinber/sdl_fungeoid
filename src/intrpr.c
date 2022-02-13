#include "intrpr.h"

Intrpr *intrpr_create()
{
    Intrpr *intrpr = (Intrpr*) malloc(sizeof(Intrpr));
    if (intrpr == NULL)
    {
        SDL_Log("Failed to malloc Intrpr\n");
        return NULL;
    }

    intrpr->_last_step_ms = 0;
    intrpr->_interval_step_ms = 100;

    intrpr->_canvas = canvas_create(128, 128);
    intrpr->_stack = stack_create();
    if (intrpr->_canvas == NULL || intrpr->_stack == NULL)
    {
        canvas_free(intrpr->_canvas);
        intrpr->_canvas = NULL;
        stack_free(intrpr->_stack);
        intrpr->_stack = NULL;

        free(intrpr);
        intrpr = NULL;

        SDL_Log("Error creating intrpr->_canvas or intrpr->_stack");
        return NULL;
    }

    // Set rest of variables
    intrpr_reset(intrpr);

    return intrpr;
}

Canvas *intrpr_get_canvas(Intrpr* intrpr)
{
    return intrpr->_canvas;
}

Stack *intrpr_get_stack(Intrpr* intrpr)
{
    return intrpr->_stack;
}

void intrpr_reset(Intrpr* intrpr)
{
    // TODO check errors of stack
    stack_clear(intrpr->_stack);
    intrpr->_ip = (SDL_Point) { 0, 0 };
    intrpr->_ip_sp = (SDL_Point) { 1, 0 };
    intrpr->_string_mode = false;
}

void intrpr_resume(Intrpr* intrpr)
{
    intrpr->_state = INTRPR_RUNNING;
}

void intrpr_pause(Intrpr* intrpr)
{
    intrpr->_state = INTRPR_PAUSED;
}

SDL_Point intrpr_get_ip(Intrpr* intrpr)
{
    return intrpr->_ip;
}

SDL_Point intrpr_get_ip_speed(Intrpr* intrpr)
{
    return intrpr->_ip_sp;
}

void intrpr_move_ip(Intrpr* intrpr, int x, int y)
{
    intrpr->_ip.x = x;
    intrpr->_ip.y = y;

    intrpr->_ip.x = juan_max(0, intrpr->_ip.x);
    intrpr->_ip.x = juan_min(canvas_get_width(intrpr->_canvas) - 1, intrpr->_ip.x);
    intrpr->_ip.y = juan_max(0, intrpr->_ip.y);
    intrpr->_ip.y = juan_min(canvas_get_height(intrpr->_canvas) - 1, intrpr->_ip.y);
}

void intrpr_move_ip_rel(Intrpr* intrpr, int x, int y)
{
    intrpr->_ip.x += x;
    intrpr->_ip.y += y;

    if (intrpr->_ip.x < 0) {
        intrpr->_ip.x = canvas_get_width(intrpr->_canvas) - 1;
    }
    if (intrpr->_ip.x > canvas_get_width(intrpr->_canvas) - 1) {
        intrpr->_ip.x = 0;
    }
    if (intrpr->_ip.y < 0) {
        intrpr->_ip.y = canvas_get_height(intrpr->_canvas) - 1;
    }
    if (intrpr->_ip.y > canvas_get_height(intrpr->_canvas) - 1) {
        intrpr->_ip.y = 0;
    }
}

enum INTRPR_STATE intrpr_get_state(Intrpr* intrpr)
{
    return intrpr->_state;
}

void intrpr_faster(Intrpr *intrpr)
{
    intrpr->_interval_step_ms /= 2;
    if (intrpr->_interval_step_ms < 20)
    {
        intrpr->_interval_step_ms = 20;
    }
}

void intrpr_slower(Intrpr *intrpr)
{
    intrpr->_interval_step_ms *= 2;
    if (intrpr->_interval_step_ms > 2000)
    {
        intrpr->_interval_step_ms = 2000;
    }
}

void intrpr_free(Intrpr *intrpr)
{
    if (intrpr != NULL)
    {
        if (intrpr->_canvas != NULL)
        {
            canvas_free(intrpr->_canvas);
            intrpr->_canvas = NULL;
        }
        if (intrpr->_stack != NULL)
        {
            stack_free(intrpr->_stack);
            intrpr->_stack = NULL;
        }
    }
    free(intrpr);
    intrpr = NULL;
}

void intrpr_step(Intrpr *intrpr)
{
    SDL_Point *ip = &(intrpr->_ip);
    SDL_Point *speed = &(intrpr->_ip_sp);
    Stack *stack = intrpr->_stack;

    // Handle better, chars can be negative
    char _c = canvas_get_char(intrpr->_canvas, ip->x, ip->y);
    char c = ' ';

    if (_c < 0) {
        SDL_Log("Error reading canvas char");
    } else {
        c = (char) _c;
    }

    enum INSTR_ID instr = const_befunge_from_char(c);

    if (intrpr->_string_mode == true)
    {
        if (c == '"') {
            intrpr->_string_mode = false;
        } else {
            stack_push(stack, (int) c);
        }
    }
    else
    {
        // TODO Check alloc errors of stack
        switch (instr)
        {
            case INSTR_0: stack_push(stack, 0); break;
            case INSTR_1: stack_push(stack, 1); break;
            case INSTR_2: stack_push(stack, 2); break;
            case INSTR_3: stack_push(stack, 3); break;
            case INSTR_4: stack_push(stack, 4); break;
            case INSTR_5: stack_push(stack, 5); break;
            case INSTR_6: stack_push(stack, 6); break;
            case INSTR_7: stack_push(stack, 7); break;
            case INSTR_8: stack_push(stack, 8); break;
            case INSTR_9: stack_push(stack, 9); break;
            case INSTR_A: stack_push(stack, 10); break;
            case INSTR_B: stack_push(stack, 11); break;
            case INSTR_C: stack_push(stack, 12); break;
            case INSTR_D: stack_push(stack, 13); break;
            case INSTR_E: stack_push(stack, 14); break;
            case INSTR_F: stack_push(stack, 15); break;

            case INSTR_UP:
                speed->x = 0;
                speed->y = -1;
                break;
            case INSTR_DOWN:
                speed->x = 0;
                speed->y = +1;
                break;
            case INSTR_LEFT:
                speed->x = -1;
                speed->y = 0;
                break;
            case INSTR_RIGHT:
                speed->x = +1;
                speed->y = 0;
                break;
            case INSTR_BRIDGE:
                ip->x += speed->x;
                ip->y += speed->y;
                break;
            case INSTR_RND:
                // Number between 0 and 3, not perfectry uniform but almost
                switch (rand() % 4)
                {
                    case 0:
                        speed->x = 0;
                        speed->y = -1;
                        break;
                    case 1:
                        speed->x = 0;
                        speed->y = +1;
                        break;
                    case 2:
                        speed->x = -1;
                        speed->y = 0;
                        break;
                    case 3:
                        speed->x = +1;
                        speed->y = 0;
                        break;
                }
                break;
            case INSTR_STOP:
                speed->x = 0;
                speed->y = 0;
                break;

            case INSTR_ADD:
                {
                    signed long int a;
                    signed long int b;
                    if (stack_pop(stack, &a) == 0
                            && stack_pop(stack, &b) == 0)
                    {
                        stack_push(stack, b + a);
                    }
                }
                break;
            case INSTR_SUB:
                {
                    signed long int a;
                    signed long int b;
                    if (stack_pop(stack, &a) == 0
                            && stack_pop(stack, &b) == 0)
                    {
                        stack_push(stack, b - a);
                    }
                }
                break;
            case INSTR_MUL:
                {
                    signed long int a;
                    signed long int b;
                    if (stack_pop(stack, &a) == 0
                            && stack_pop(stack, &b) == 0)
                    {
                        stack_push(stack, b * a);
                    }
                }
                break;
            case INSTR_INTDIV:
                {
                    signed long int a;
                    signed long int b;
                    if (stack_pop(stack, &a) == 0
                            && stack_pop(stack, &b) == 0)
                    {
                        stack_push(stack, b / a);
                    }
                }
                break;
            case INSTR_MOD:
                {
                    signed long int a;
                    signed long int b;
                    if (stack_pop(stack, &a) == 0
                            && stack_pop(stack, &b) == 0)
                    {
                        stack_push(stack, b % a);
                    }
                }
                break;
            case INSTR_NOT:
                {
                    signed long int a;
                    if (stack_pop(stack, &a) == 0)
                    {
                        if (a == 0)
                        {
                            stack_push(stack, 1);
                        }
                        else
                        {
                            stack_push(stack, 0);
                        }
                    }
                }
                break;
            case INSTR_GT:
                {
                    signed long int a;
                    signed long int b;
                    if (stack_pop(stack, &a) == 0
                            && stack_pop(stack, &b) == 0)
                    {
                        if (b > a)
                        {
                            stack_push(stack, 1);
                        }
                        else
                        {
                            stack_push(stack, 0);
                        }
                    }
                }
                break;
            case INSTR_LT:
                {
                    signed long int a;
                    signed long int b;
                    if (stack_pop(stack, &a) == 0
                            && stack_pop(stack, &b) == 0)
                    {
                        if (b < a)
                        {
                            stack_push(stack, 1);
                        }
                        else
                        {
                            stack_push(stack, 0);
                        }
                    }
                }
                break;

            case INSTR_DUP:
                {
                    signed long int v;
                    if (stack_pop(stack, &v) == 0)
                    {
                        stack_push(stack, v);
                        stack_push(stack, v);
                    }
                }
                break;
            case INSTR_SWP:
                {
                    signed long int a;
                    signed long int b;
                    if (stack_pop(stack, &a) == 0
                            && stack_pop(stack, &b) == 0)
                    {
                        stack_push(stack, a);
                        stack_push(stack, b);
                    }
                }
                break;
            case INSTR_POP:
                {
                    signed long int v;
                    stack_pop(stack, &v);
                }
                break;
            case INSTR_HIF:
                {
                    signed long int a;
                    if (stack_pop(stack, &a) == 0)
                    {
                        if (a == 0)
                        {
                            speed->x = +1;
                            speed->y = 0;
                        }
                        else
                        {
                            speed->x = -1;
                            speed->y = 0;
                        }
                    }
                }
                break;
            case INSTR_VIF:
                {
                    signed long int a;
                    if (stack_pop(stack, &a) == 0)
                    {
                        if (a == 0)
                        {
                            speed->x = 0;
                            speed->y = +1;
                        }
                        else
                        {
                            speed->x = 0;
                            speed->y = -1;
                        }
                    }
                }
                break;
            case INSTR_STR:
                intrpr->_string_mode = true;
                break;
            case INSTR_ITER:
                // TODO show on screen some indicator of the action being made
                break;

            case INSTR_INTIN:
                // TODO
                break;
            case INSTR_CHARIN:
                // TODO
                break;
            case INSTR_INTOUT:
                {
                    signed long int v;
                    if (stack_pop(stack, &v) == 0)
                    {
                        SDL_Log("INTOUT: %lu", v);
                    }
                }
                break;
            case INSTR_CHAROUT:
                {
                    signed long int v;
                    if (stack_pop(stack, &v) == 0)
                    {
                        SDL_Log("CHAROUT: %lx", v);
                        // TODO is printing hex instead of char
                    }
                }
                break;

            case INSTR_GET:
                {
                    signed long int y;
                    signed long int x;
                    if (stack_pop(stack, &y) == 0
                            && stack_pop(stack, &x) == 0)
                    {
                        char c = canvas_get_char(intrpr->_canvas, x, y);
                        if (c >= 0) {
                            stack_push(stack, (signed long int) c);
                        }
                    }
                }
                break;
            case INSTR_PUT:
                {
                    signed long int y;
                    signed long int x;
                    signed long int v;
                    if (stack_pop(stack, &y) == 0
                            && stack_pop(stack, &x) == 0
                            && stack_pop(stack, &v) == 0)
                    {
                        canvas_set_char(intrpr->_canvas, x, y, (char) v);
                    }
                }
                break;
            case INSTR_FETCH:
                {
                    ip->x += speed->x;
                    ip->y += speed->y;
                    char c = canvas_get_char(intrpr->_canvas, ip->x, ip->y);
                    if (c >= 0) {
                        stack_push(stack, (signed long int) c);
                    }
                }
                break;
            case INSTR_STORE:
                {
                    ip->x += speed->x;
                    ip->y += speed->y;
                    signed long int v;
                    if (stack_pop(stack, &v) == 0)
                    {
                        canvas_set_char(intrpr->_canvas, ip->x, ip->y, (char) v);
                    }
                }
                break;

            case INSTR_NULL:
                SDL_Log("Found null instruction");
                break;

            default:
                break;
        }
    }

    ip->x += speed->x;
    ip->y += speed->y;
}

void intrpr_update(Intrpr *intrpr, Uint32 time_abs_ms)
{
    if (intrpr->_state == INTRPR_RUNNING)
    {
        // TODO: Do several steps if we are very late, maybe maximim 10 in case
        // we reach processor limit
        if (time_abs_ms - intrpr->_last_step_ms > intrpr->_interval_step_ms)
        {
            intrpr->_last_step_ms = time_abs_ms;
            intrpr_step(intrpr);
        }
    }
}

