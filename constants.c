/*
 * constants.c
 *
 * Sprite metrics constants.
 */
#include "soda.h"

const struct metrics c_Metrics[ID_T_MAX] = {

    // NOTHING
    {
        .filename      = "NOTHING",
        .h_frame_shift = 0,
        .v_frame_shift = 0,
        .box_width     = 0,
        .box_height    = 0,
        .strength      = -1
    },

    // PLAYER
    {
        .filename      = "doom.bmp",
        .h_frame_shift =   6,
        .v_frame_shift =   6,
        .box_width     =  35,
        .box_height    =  55,
        .strength      =  20
    },

    // TROOPER
    {
        .filename      = "trooper.bmp",
        .h_frame_shift =   6,
        .v_frame_shift =   6,
        .box_width     =  35,
        .box_height    =  55,
        .strength      =   1
    },
    
    // DEMON
    {
        .filename      = "demon.bmp",
        .h_frame_shift =   6,
        .v_frame_shift =   6,
        .box_width     =  35,
        .box_height    =  55,
        .strength      =   3
    },

    // BULL
    {
        .filename      = "bull.bmp",
        .h_frame_shift =   6,
        .v_frame_shift =   7,
        .box_width     =  45,
        .box_height    =  70,
        .strength      =   5
    },

    // CYBERBULL
    {
        .filename      = "cyberbull.bmp",
        .h_frame_shift =   7,
        .v_frame_shift =   7,
        .box_width     =  75,
        .box_height    = 105,
        .strength      =  10
    },

    // PLAYER_ATTACK
    {
        .filename      = "doom_attack.bmp",
        .h_frame_shift =   6,
        .v_frame_shift =   6,
        .box_width     =  49,
        .box_height    =  55,
        .strength      =   1
    },

    // TROOPER_ATTACK
    {
        .filename      = "trooper_attack.bmp",
        .h_frame_shift =   5,
        .v_frame_shift =   5,
        .box_width     =  32,
        .box_height    =  55,
        .strength      =   1
    },

    // DEMON_ATTACK
    {
        .filename      = "demon_attack.bmp",
        .h_frame_shift =   5,
        .v_frame_shift =   5,
        .box_width     =  32,
        .box_height    =  25,
        .strength      =   2
    },

    // BULL_ATTACK
    {
        .filename      = "bull_attack.bmp",
        .h_frame_shift =   6,
        .v_frame_shift =   7,
        .box_width     =  45,
        .box_height    =  75,
        .strength      =   3
    },

    // CYBERBULL_ATTACK
    {
        .filename      = "cyberbull_attack.bmp",
        .h_frame_shift =   6,
        .v_frame_shift =   6,
        .box_width     =  64,
        .box_height    =  55,
        .strength      =   4
    }

};

