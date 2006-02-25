/*
 * soda.c
 *
 * Main function.
 */
#include "soda.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <SDL.h>


int main (int argc, char **argv)
{
    Uint32 frame_start, frame_end;
    int e, fullscreen;

    srand( (unsigned int) time( NULL ) );

    e = SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER );
    if( e < 0 ) {
        // Don't use Fatal() because there's no need to call SDL_Quit()
        fputs( "S.O.D. ERROR: Colud not initialize SDL.\n", stderr );
        fputs( "SDL ERROR: ", stderr );
        fputs( SDL_GetError(), stderr );
        fputc( '\n', stderr );
        return EXIT_FAILURE;
    }

    fullscreen = 0;
    if( (argc > 1) && (strcmp( argv[1], "-f" ) == 0) )
        fullscreen = 1;

    SDL_WM_SetCaption( "Sreets Of Doom Arena", NULL );

    InitGraphics( fullscreen );
    InitGame();

    // Main game loop
    for (;;) {
        frame_start = SDL_GetTicks();

        ProcessEvents();
        RoamEnemies();
        AnimateScene();
        DrawScene();

        // Synchronize frame
        do {
            frame_end = SDL_GetTicks();
        } while( (frame_end - frame_start) < FRAME_DELAY );
    }
        
    fputs( "You shouldn't be reading this, check your code.\n", stderr );
    Quit();
    return EXIT_FAILURE;
}

