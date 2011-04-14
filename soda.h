/*
 * soda.h
 *
 * Types and functions used in the game.
 */
#ifndef __sod_h
#define __sod_h
#include <SDL.h>

/****************************************************************************/
/*                               CUSTOM TYPES                               */
/****************************************************************************/
/*
 * game_state_t: Game state values.
 */
typedef enum
{
        INITIAL      = 0,
        PLAYING      = 1,
        ROUND_CHANGE = 2,
        GAME_OVER    = 3
} game_state_t;


/*
 * id_t: Number that identifies a drawable object on the screen, except for
 *       the map.
 */
typedef enum
{
        NOTHING          =  0,
        PLAYER           =  1,
        TROOPER          =  2,
        DEMON            =  3,
        BULL             =  4,
        CYBERBULL        =  5,
        PLAYER_ATTACK    =  6,  // Painful sprites
        TROOPER_ATTACK   =  7,
        DEMON_ATTACK     =  8,
        BULL_ATTACK      =  9,
        CYBERBULL_ATTACK = 10,
        ID_T_MAX         = 11   // Leave this one as the last
} id_t;


/*
 * action_t: Enum to hold a possible action of an item.
 */
typedef enum
{
        NONE         = 0, // Don't display it
        AIMING       = 1, // Shooting pose
        MOVING       = 2,
        SHOOTING     = 3, // Firing
        HIT          = 4,
        DYING        = 5,
        ACTION_T_MAX = 6
} action_t;


/*
 * item_t: Struct that represents the complete state of an item on the
 *          screen.
 */
typedef struct
{
        // State info
        id_t     type;
        action_t action;
        action_t previous_action;
        // Displacement info
        int  direction;
        int  x_pos;
        int  z_pos;
        int  h_increment;
        int  v_increment;
        // Sprite metrics
        int  box_width;
        int  box_height;
        int  h_frame_shift;
        int  v_frame_shift;
        // Misc info
        int  overlay;     // Overlay object occuping the same cell
        int  strength;    // Remainig hits until death, for attacks is pain effect
        // Animation info
        unsigned int delay;   // Animation info
        unsigned int frame;
} item_t;

/*
 * Include constants too. They are separated for easier customization.
 */
#include "constants.h"

/*** EXPORTED GLOBALS ***/
extern game_state_t g_GameState;
extern item_t       g_Item[ITEMS_MAX];
extern int          g_Cell[ROWS_MAX][COLS_MAX];
extern int          g_Round;
extern int          g_RoundEnemiesLeft;


/****************************************************************************/
/*                           FUNCTION PROTOTYPES                            */
/****************************************************************************/

/*** graphics.c ***/
extern void InitGraphics    (int);
extern void ClearScreen     (void);
extern void FlushScreen     (void);
extern void DrawItem        (item_t *);
extern void DrawHealth      (item_t *);
extern void CleanUpGraphics (void);

/*** input.c ***/
extern void ProcessEvents (void);

/*** scene.c ***/
extern void ClearScene   (void);
extern void AnimateScene (void);
extern void DrawScene    (void);

/*** item.c ***/
extern void NewItem   (id_t, int, int, int);
extern void NewAttack (item_t *);
extern void DoHit     (int, int, int, int);

/*** game.c ***/
extern void InitGame      (void);
extern void LoadNextRound (void);
extern void RoamEnemies   (void);
extern void CleanUpGame   (void);

/*** misc.c ***/
extern void   Quit      (void);
extern void   Warning   (const char *, ...);
extern void   Fatal     (const char *, ...);
extern Uint32 ChangeState (Uint32, void *);
extern char  *MediaFile (const char *);

#endif /* __prototypes_h */

