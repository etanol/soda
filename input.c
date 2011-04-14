/*
 * input.c
 *
 * Various functions to get keyboard input.
 */
#include "soda.h"
#include <SDL.h>

extern item_t g_Item[ITEMS_MAX];


/***************************  PRIVATE FUNCTIONS  ***************************/
/*
 * HandleKeyDown - Handle keyboard press of the given key. Private function.
 */
static void HandleKeyDown (SDLKey key)
{
        item_t *player;

        player = &g_Item[0];

        switch (g_GameState)
        {
        case INITIAL:
                switch (key)
                {
                case SDLK_RETURN:
                        g_Round = 0;
                        LoadNextRound();
                        break;
                case SDLK_ESCAPE:
                        Quit();
                default:
                        break;
                }
                break;

        case PLAYING:
                switch (key)
                {
                case SDLK_LEFT:
                        if (player->action != DYING)
                        {
                                player->action      = MOVING;
                                player->h_increment = -PLAYER_HORIZ_SPEED;
                                player->direction  |= WEST;
                        }
                        break;
                case SDLK_RIGHT:
                        if (player->action != DYING)
                        {
                                player->action      = MOVING;
                                player->h_increment = PLAYER_HORIZ_SPEED;
                                player->direction  &= ~WEST;
                        }
                        break;
                case SDLK_DOWN:
                        if (player->action != DYING)
                        {
                                player->action      = MOVING;
                                player->v_increment = PLAYER_VERT_SPEED;
                                player->direction  &= ~NORTH;
                        }
                        break;
                case SDLK_UP:
                        if (player->action != DYING)
                        {
                                player->action      = MOVING;
                                player->v_increment = -PLAYER_VERT_SPEED;
                                player->direction  |= NORTH;
                        }
                        break;
                case SDLK_RCTRL:
                case SDLK_LCTRL:
                        if (player->action != DYING)
                                NewAttack(player);
                        break;
                case SDLK_ESCAPE:
                        Quit();
                default:
                        break;
                }
        default:
                break;
        }
}


/*
 * HandleKeyUp - Handle keybaord release of the given key. Private function.
 */
static void HandleKeyUp (SDLKey key)
{
        item_t *player;

        player = &g_Item[0];

        if (player->action == DYING)
                return;

        switch (key)
        {
        case SDLK_LEFT:
                // Only stop if release matches current direction
                if (player->h_increment < 0)
                {
                        player->h_increment = 0;
                        if (player->v_increment == 0)
                                player->action = AIMING;
                }
                break;
        case SDLK_RIGHT:
                if (player->h_increment > 0)
                {
                        player->h_increment = 0;
                        if (player->v_increment == 0)
                                player->action = AIMING;
                }
                break;
        case SDLK_DOWN:
                if (player->v_increment > 0)
                {
                        player->v_increment = 0;
                        player->direction &= ~NORTH;
                        if (player->h_increment == 0)
                                player->action = AIMING;
                }
                break;
        case SDLK_UP:
                if (player->v_increment < 0)
                {
                        player->v_increment = 0;
                        player->direction &= ~NORTH;
                        if (player->h_increment == 0)
                                player->action = AIMING;
                }
                break;
        default:
                break;
        }
}


/****************************  PUBLIC FUNCTIONS  ****************************/
/*
 * ProcessEvents - Read pending events in the event cue and process them
 *                 dispatching to the helper functions if required.
 */
void ProcessEvents (void)
{
        SDL_Event event;

        while (SDL_PollEvent(&event))
                switch (event.type)
                {
                case SDL_KEYDOWN:
                        HandleKeyDown(event.key.keysym.sym);
                        break;

                case SDL_KEYUP:
                        HandleKeyUp(event.key.keysym.sym);
                        break;

                case SDL_QUIT:
                        Quit();

                default:
                        break;
                }
}

