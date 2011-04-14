/*
 * constants.h
 *
 * Constants and other obscure macros.
 */
#ifndef __sod_h
#error "Don't include \"constants.h\" directly. Use \"sod.h\" instead."
#endif

#ifndef CHAR_BITS
#define CHAR_BITS  8
#endif
#define SIGN_SHIFT ((sizeof( int ) * CHAR_BITS) - 1)

#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600
#define SCREEN_DEPTH    16  // Bits per pixel

#define Z_MIN  110
#define Z_MAX  590

#define ITEMS_MAX       128 // Maximum number of items per map
#define ATTACK_FIRST     64
#define ATTACK_MASK   0x03F

#define DATA_DIRECTORY  "stuff"
#define PATH_MAX        64

#define SPLASH_DELAY    2000 // In miliseconds
#define FRAME_DELAY     30   // In miliseconds
#define SHOOTING_DELAY   8   // In frames
#define HIT_DELAY       12   // In frames

#define NORTH     0x01
#define WEST      0x02

#define CELL_WIDTH   32
#define CELL_HEIGHT  32
#define ROWS_MAX     (((Z_MAX - Z_MIN) / CELL_HEIGHT) + 1)
#define COLS_MAX     (SCREEN_WIDTH / CELL_WIDTH)

#define PLAYER_VERT_SPEED    4
#define PLAYER_HORIZ_SPEED   4
#define PLAYER_ATTACK_SPEED 16
#define ENEMY_VERT_SPEED     2
#define ENEMY_HORIZ_SPEED    2
#define ENEMY_ATTACK_SPEED  10

#define CENTER_WIDTH(x)  ((SCREEN_WIDTH / 2) - ((x) / 2))
#define CENTER_HEIGHT(x) ((SCREEN_HEIGHT / 2) - ((x) / 2))
#define FONT_WIDTH          17
#define FONT_HEIGHT         36
#define FONT_GO_WIDTH      161   // Game over text
#define FONT_RO_WIDTH      161   // Round text
#define FONT_ZERO            2   // Row where '0' is

#define HEALTH_XPOS         10
#define HEALTH_YPOS         10
#define HEALTH_HEIGHT       20

struct metrics
{
        char *filename;
        int   h_frame_shift;
        int   v_frame_shift;
        int   box_width;
        int   box_height;
        int   strength;
};

extern const struct metrics c_Metrics[ID_T_MAX];

