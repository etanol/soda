/*
 * item.c
 *
 * Scene items creation.
 */
#include "soda.h"
#include <stdlib.h>

/*****************************  MODULE GLOBALS  *****************************/
item_t g_Item[ITEMS_MAX];  // Exported
int    m_FreeItem;


/****************************  PUBLIC FUNCTIONS  ****************************/
/*
 * NewItem - Initializes item at i position. Also updates the cell.
 */
void NewItem (id_t type, int index, int row, int col)
{
        if (g_Cell[row][col] >= 0)
        {
                Warning("Overlapping creation requested at (%d, %d).", row, col);
                return;
        }

        g_Cell[row][col] = index;

        g_Item[index].type        = type;
        g_Item[index].action      = AIMING;
        g_Item[index].x_pos       = col * CELL_WIDTH;
        g_Item[index].z_pos       = (row * CELL_HEIGHT) + Z_MIN;
        g_Item[index].h_increment = 0;
        g_Item[index].v_increment = 0;
        g_Item[index].overlay     = -1;
        g_Item[index].delay       = 0;
        g_Item[index].frame       = 0;
        g_Item[index].direction   = 0;
        if (col > (COLS_MAX / 2))
                g_Item[index].direction = WEST;

        g_Item[index].h_frame_shift = c_Metrics[type].h_frame_shift;
        g_Item[index].v_frame_shift = c_Metrics[type].v_frame_shift;
        g_Item[index].box_width     = c_Metrics[type].box_width;
        g_Item[index].box_height    = c_Metrics[type].box_height;
        g_Item[index].strength      = c_Metrics[type].strength;
}


/*
 * NewAttack - Creates a new attack from the specified item.
 */
void NewAttack (item_t *item)
{
        int  row, col, col0, i;
        int  speed;
        id_t new_type;

        // Get location info
        col0 =  item->x_pos / CELL_WIDTH;
        col  = (item->x_pos / CELL_WIDTH) + (1 - (item->direction & WEST));
        row  = (item->z_pos - Z_MIN) / CELL_HEIGHT;

        // Source attacker has to do it graphically
        if (item->action != SHOOTING)
        {
                item->previous_action = item->action;
                item->action          = SHOOTING;
        }
        item->delay = 0;
        item->frame = 0;

        // Next to an alive object, hit directly
        if (g_Cell[row][col] >= 0)
        {
                DoHit(row, col0, row, col);
                return;
        }

        // Get a new item slot and reserve its cell
        i        = ATTACK_FIRST + m_FreeItem;
        new_type = item->type + 5;
        speed    = item->type==PLAYER ? PLAYER_ATTACK_SPEED : ENEMY_ATTACK_SPEED;
        g_Cell[row][col] = i;

        g_Item[i].type      = new_type;
        g_Item[i].action    = MOVING;
        g_Item[i].direction = item->direction;
        g_Item[i].z_pos     = item->z_pos;
        g_Item[i].x_pos     = (col * CELL_WIDTH)  // Too lazy to explain it
                              + ((CELL_WIDTH-1) & ((col - col0) >> SIGN_SHIFT));
        g_Item[i].delay     = 0;
        g_Item[i].frame     = 0;
        g_Item[i].overlay   = -1;

        g_Item[i].strength      = c_Metrics[new_type].strength;
        g_Item[i].box_width     = c_Metrics[new_type].box_width;
        g_Item[i].box_height    = c_Metrics[new_type].box_height;
        g_Item[i].h_frame_shift = c_Metrics[new_type].h_frame_shift;
        g_Item[i].v_frame_shift = c_Metrics[new_type].v_frame_shift;

        g_Item[i].v_increment = 0;
        if (item->direction & WEST)
                g_Item[i].h_increment = -speed;
        else
                g_Item[i].h_increment = speed;

        m_FreeItem = (m_FreeItem + 1) & ATTACK_MASK;
}


/*
 * DoHit - Make a hit effective from cell source position to destination.
 */
void DoHit (int row_src, int col_src, int row_dst, int col_dst)
{
        int src, dst;

        src = g_Cell[row_src][col_src];
        dst = g_Cell[row_dst][col_dst];

        if (g_Item[src].type >= PLAYER_ATTACK)
        {
                // Source is an attack
                g_Cell[row_src][col_src] = -1;

                g_Item[src].action    = DYING;
                g_Item[src].delay     = 0;
                g_Item[src].frame     = 0;
                g_Item[src].overlay   = dst;
                g_Item[dst].overlay   = src;
        }
        else
        {
                // Origin is next to destination
                int tmp;

                tmp = ATTACK_FIRST + m_FreeItem;

                // Create an exploding overlay
                g_Item[tmp].type      = g_Item[src].type + 5;
                g_Item[tmp].action    = DYING;
                g_Item[tmp].x_pos     = g_Item[dst].x_pos;
                g_Item[tmp].z_pos     = g_Item[dst].z_pos;
                g_Item[tmp].delay     = 0;
                g_Item[tmp].frame     = 0;
                g_Item[tmp].overlay   = dst;
                g_Item[dst].overlay   = tmp;

                // Very dirty, please close your eyes and skip this page
                src = tmp;
                tmp = g_Item[src].type;
                g_Item[src].h_frame_shift = c_Metrics[tmp].h_frame_shift;
                g_Item[src].v_frame_shift = c_Metrics[tmp].v_frame_shift;
                g_Item[src].box_width     = c_Metrics[tmp].box_width;
                g_Item[src].box_height    = c_Metrics[tmp].box_height;
                g_Item[src].strength      = c_Metrics[tmp].strength;

                m_FreeItem = (m_FreeItem + 1) & ATTACK_MASK;
        }

        g_Item[dst].strength -= g_Item[src].strength;

        if (g_Item[dst].strength <= 0)
        {
                if (g_Item[dst].action != DYING)
                {
                        g_Item[dst].action  = DYING;
                        g_Item[dst].delay   = 0;
                        g_Item[dst].frame   = 0;
                }
        }
        else
        {
                switch (g_Item[dst].action)
                {
                case MOVING:
                case AIMING:
                        g_Item[dst].previous_action = g_Item[dst].action;
                        // Fall through
                default:
                        g_Item[dst].action = HIT;
                        g_Item[dst].delay  = 0;
                        g_Item[dst].frame  = 0;
                        break;
                }
        }
}

