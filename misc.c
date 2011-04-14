/*
 * misc.c
 *
 * Unclassiffied auxiliary functions.
 */
#include "soda.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>


/***************************  PRIVATE FUNCTIONS  ***************************/
/*
 * CleanUp - Private function to clean up everything before exiting.
 */
static void CleanUp (void)
{
        CleanUpGame();
        CleanUpGraphics();
        SDL_Quit();
}


/****************************  PUBLIC FUNCTIONS  ****************************/
/*
 * Quit - Clean up everything and exit successfully.
 */
void Quit (void)
{
        CleanUp();
        exit(EXIT_SUCCESS);
}


/*
 * Warning - Print a warning, a la printf, along with the SDL error but do not
 *           abort.
 */
void Warning (const char *msg, ...)
{
        va_list args;

        fputs("S.O.D. WARNING: ", stderr);
        va_start(args, msg);
        vfprintf(stderr, msg, args);
        va_end(args);
        fprintf(stderr, "\nSDL ERROR: %s\n", SDL_GetError());
}


/*
 * Fatal - Print a custom message (printf style) on stderr and abort. The SDL
 *         error is also printed.
 */
void Fatal (const char *msg, ...)
{
        va_list args;

        fputs("S.O.D. ERROR: ", stderr);
        va_start(args, msg);
        vfprintf(stderr, msg, args);
        va_end(args);
        fprintf(stderr, "\nSDL ERROR: %s\n", SDL_GetError());
        CleanUp();
        exit(EXIT_FAILURE);
}


/*
 * MediaFile - Build the pathame to open name sucessfully, assuming that name
 *             references some kind of static media: sprites, audio...
 */
char *MediaFile (const char *name)
{
        static char media_path[PATH_MAX];

        snprintf(media_path, PATH_MAX, "%s/%s", DATA_DIRECTORY, name);
        return media_path;
}


/*
 * ChangeState - Callback function to set another game state.
 */
Uint32 ChangeState (Uint32 interval, void *param)
{
        switch (g_GameState)
        {
        case ROUND_CHANGE:
                g_GameState = PLAYING;
                break;
        case GAME_OVER:
                g_GameState = INITIAL;
        default:
                break;
        }
        return 0;
}

