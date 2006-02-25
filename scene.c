/*
 * scene.c
 *
 * Scene management module: animation and painting.
 */
#include "soda.h"
#include <string.h>


/*****************************  MODULE GLOBALS  *****************************/
int g_Cell[ROWS_MAX][COLS_MAX]; // Exported


/************************  INLINE UTILITY FUNCTIONS  ************************/
/*
 * FastMin - Computes the minimum of x and y without branching.
 */
static inline int FastMin (int x, int y)
{
    int d;

    d = x - y;
    return y + (d & (d >> SIGN_SHIFT));
}


/*
 * FastMax - Computes de maximum of x and y without branching.
 */
static inline int FastMax (int x, int y)
{
    int d;

    d = x - y;
    return x - (d & (d >> SIGN_SHIFT));
}


/*
 * FastAbs - Computes the absolute value of x without branching.
 */
static inline int FastAbs (int x)
{
    int sign;

    sign = x >> SIGN_SHIFT;
    return (x ^ sign) - sign;
}


/****************************  PUBLIC FUNCTIONS  ****************************/
/*
 * ClearScene - This function prepares you a cup of coffee.
 */
void ClearScene (void)
{
    memset( &g_Cell[0][0], -1, (COLS_MAX * ROWS_MAX) * sizeof( int ) );
    memset( &g_Item[0],     0, ITEMS_MAX * sizeof( item_t ) );
}


/*
 * AnimateScene - Move all active objects. This function also takes care of
 *                collisions.
 */
void AnimateScene (void)
{
    int i, row0, col0, cmp;
    int x, z, row, col;
    item_t *item;

    if( g_GameState != PLAYING )
        return;

    // Animation is done for every item
    for( i = 0; i < ITEMS_MAX; i++ ) {
        if( g_Item[i].type == NOTHING ) 
            continue;

        item = &g_Item[i];
        switch( item->action ) {
            case MOVING:
                // Displace with bounds checking
                x = item->x_pos + item->h_increment;
                x = FastMin( x, SCREEN_WIDTH - item->box_width );
                x = FastMax( x, 0 );
                z = item->z_pos + item->v_increment;
                z = FastMin( z, Z_MAX );
                z = FastMax( z, Z_MIN );

                // Compute cell and check collisions
                col0 = item->x_pos / CELL_WIDTH;
                row0 = (item->z_pos - Z_MIN) / CELL_HEIGHT;
                col  = x / CELL_WIDTH;
                row  = (z - Z_MIN) / CELL_HEIGHT;
                cmp  = FastAbs( col - col0 ) + FastAbs( row - row0 );

                if( (cmp == 0) || (g_Cell[row][col] < 0) ) {
                    if( (item->type >= PLAYER_ATTACK) &&
                            ((item->x_pos == 0) ||(item->x_pos == (SCREEN_WIDTH-item->box_width))) ) {
                        item->type       = NOTHING;
                        g_Cell[row][col] = -1;
                    } else {
                        g_Cell[row0][col0] = -1;
                        g_Cell[row][col]   = i;
                        item->x_pos = x;
                        item->z_pos = z;
                    }
                } else {
                    // Collision detected, check if it's a hit
                    if( item->type >= PLAYER_ATTACK )
                        DoHit( row0, col0, row, col );
                }

                // Compute next frame respecting the delay
                item->delay++;
                item->frame += item->delay >> 2;
                item->delay &= 0x03;
                break;

            case SHOOTING:
                item->delay++;
                if( item->delay > SHOOTING_DELAY )
                    item->action = item->previous_action;
                break;
            case HIT:
                item->delay++;
                if( item->delay > HIT_DELAY )
                    item->action = item->previous_action;
                break;

            case DYING:
                col = item->x_pos / CELL_WIDTH;
                row = (item->z_pos - Z_MIN) / CELL_HEIGHT;
                item->delay++;

                if( item->type >= PLAYER_ATTACK ) {
                    item->frame += item->delay >> 1;
                    item->delay &= 0x01;
                    if( item->frame >= 4 ) {
                        item->type = NOTHING;
                        if( g_Cell[row][col] == i ) 
                            g_Cell[row][col] = -1;
                        else
                            g_Item[item->overlay].overlay = -1;
                    }
                } else {
                    item->frame += item->delay >> 2;
                    item->delay &= 0x03;
                    if( item->frame >= 8 ) {
                        if( item->type == PLAYER ) {
                            g_GameState = GAME_OVER;
                            SDL_AddTimer( SPLASH_DELAY, ChangeState, NULL );
                        } else {
                            g_RoundEnemiesLeft--;
                            if( g_RoundEnemiesLeft == 0) {
                                LoadNextRound();
                                return;
                            }
                        }
                        item->type = NOTHING;
                        g_Cell[row][col] = -1;
                    }
                }
                // Fall through

            default:
                break;
        }

    }
}


/*
 * DrawScene - Paint the screen completely.
 */
void DrawScene (void)
{
    int i, c;

    ClearScreen();

    if( g_GameState == PLAYING ) {
        for( c = 0; c < (COLS_MAX * ROWS_MAX); c++ ) {
            i = g_Cell[0][c];
            if( i >= 0 ) {
                DrawItem( &g_Item[i] );
                if( g_Item[i].overlay >= 0 )
                    DrawItem( &g_Item[g_Item[i].overlay] );
            }
        }
    }

    DrawHealth( &g_Item[0] );

    FlushScreen();
}

