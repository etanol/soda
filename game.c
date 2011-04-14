/*
 * game.c
 *
 * Enemies intelligence and minimal game management.
 */
#include "soda.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#define LINE_WIDTH 256

/*****************************  MODULE GLOBALS  *****************************/
game_state_t g_GameState; // Exported
int          g_RoundEnemiesLeft;
int          g_Round;
FILE        *m_RoundData;
char         m_Line[LINE_WIDTH];


/************************  INLINE UTILITY FUNCTIONS  ************************/
/*
 * UpdateDirection - Generates proper horizontal and vertical displacement
 *                   according the direction given.
 */
static inline void UpdateDirection (item_t *item, int direction)
{
        switch (direction)
        {
        case 0:
                item->h_increment =  ENEMY_VERT_SPEED;
                item->v_increment =  ENEMY_HORIZ_SPEED;
                break;
        case NORTH:
                item->h_increment =  ENEMY_VERT_SPEED;
                item->v_increment = -ENEMY_HORIZ_SPEED;
                break;
        case WEST:
                item->h_increment = -ENEMY_VERT_SPEED;
                item->v_increment =  ENEMY_HORIZ_SPEED;
                break;
        case NORTH|WEST:
                item->h_increment = -ENEMY_VERT_SPEED;
                item->v_increment = -ENEMY_HORIZ_SPEED;
                break;
        }
        item->direction = direction;
}


/****************************  PUBLIC FUNCTIONS  ****************************/
/*
 * InitGame - Load rounds file.
 */
void InitGame (void)
{
        m_RoundData = fopen(MediaFile("round_list.txt"), "rt");
        if (m_RoundData == NULL)
                Fatal("Could not load 'round_list.txt'.");
        g_GameState = INITIAL;
}


/*
 * LoadNextRound - Reads rounds file and load the scene. Syntax validation of
 *                 the file is quite weak.
 */
void LoadNextRound (void)
{
        char *e;
        int  i, dummy, row, col;
        id_t type;

        if (g_Round == 0)
                rewind(m_RoundData);

        g_Round++;
        ClearScene();
        NewItem(PLAYER, 0, ROWS_MAX / 2, COLS_MAX / 2);
        i = 1;

        do {
                e = fgets(m_Line, LINE_WIDTH, m_RoundData);
                if (e == NULL)
                {
                        // End Of File, no more rounds
                        g_GameState  = GAME_OVER;
                        SDL_AddTimer(SPLASH_DELAY, ChangeState, NULL);
                        return;

                }
                else if (m_Line[0] == '%')
                {

                        // End of Round
                        g_RoundEnemiesLeft = i - 1;
                        g_GameState        = ROUND_CHANGE;
                        SDL_AddTimer(SPLASH_DELAY, ChangeState, NULL);
                        return;

                }
                else if (m_Line[0] != '#')
                {
                        // Read data
                        sscanf(m_Line, "%d %d %d", &dummy, &row, &col);
                        type = dummy;
                        if ((type > PLAYER) && (type < PLAYER_ATTACK))
                        {
                                NewItem(type, i, row, col);
                                i++;
                        }

                }
        } while (1);
}


/*
 * RoamEnemies - Set actions to enemies according its AI.
 */
void RoamEnemies (void)
{
        int player_row, player_col, row, col;
        int i, direction, chasing_frequency;
        item_t *item;

        if (g_GameState != PLAYING)
                return;

        player_col = g_Item[0].x_pos / CELL_WIDTH;
        player_row = (g_Item[0].z_pos - Z_MIN) / CELL_HEIGHT;

        for (i = 1;  i < ATTACK_FIRST;  ++i)
        {
                if (g_Item[i].type == NOTHING)
                        continue;

                item = &g_Item[i];

                col = item->x_pos / CELL_WIDTH;
                row = (item->z_pos - Z_MIN) / CELL_HEIGHT;

                switch (item->action)
                {
                case SHOOTING:
                case HIT:
                case DYING:
                        // These states are blocking
                        break;
                default:
                        if (row == player_row)
                        {
                                // Leave some shooting gaps to scape but keep
                                // the position
                                direction        = item->direction;
                                item->action     = AIMING;
                                item->direction  = 0;
                                item->direction  = direction & NORTH;
                                item->direction |= WEST & ((player_col - col) >> SIGN_SHIFT);
                                if (((rand() >> 8) & 0x0F) == 5)
                                        NewAttack(item);
                        }
                        else
                        {
                                switch (item->type)
                                {
                                        case TROOPER:   chasing_frequency = 20; break;
                                        case DEMON:     chasing_frequency = 10; break;
                                        case BULL:      chasing_frequency =  5; break;
                                        case CYBERBULL: chasing_frequency =  3; break;
                                        default:        return;
                                }

                                if (item->action != MOVING || item->frame > chasing_frequency)
                                {
                                        // Go hunt player
                                        if (item->action != MOVING || item->frame > 20)
                                        {
                                                direction = (WEST & ((player_col - col) >> SIGN_SHIFT))
                                                            | (NORTH & ((player_row - row) >> SIGN_SHIFT));
                                                UpdateDirection(item, direction);
                                                item->delay  = 0;
                                                item->frame  = 0;
                                                item->action = MOVING;
                                        }
                                }
                        }
                }
        }
}


/*
 * CleanUpGame - Close rounds file.
 */
void CleanUpGame (void)
{
        fclose(m_RoundData);
}

