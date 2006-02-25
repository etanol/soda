/*
 * graphics.c
 *
 * Module for loading sprites and painting on to the screen. Note that sprite
 * management is done privately. Sprites outside the module are seen as
 * integer handle IDs which are used to reference the sprite table.
 */
#include "soda.h"
#include <string.h>
#include <SDL.h>


/*****************************  MODULE GLOBALS  *****************************/
SDL_Surface *m_Screen;
SDL_Surface *m_Background;
SDL_Surface *m_Splash;
SDL_Surface *m_Font;
SDL_Surface *m_Sprite[ID_T_MAX];
Uint32       m_ColorKey;
Uint32       m_ClearColor;
Uint32       m_HealthColor;


/************************  INLINE UTILITY FUNCTIONS  ************************/
/*
 * SetRect - Sets the fields of an SDL_Rect in a handy way.
 */
static inline void SetRect (SDL_Rect *r, int x, int y,
                            unsigned int w, unsigned int h)
{
    r->x = x;  // Left
    r->y = y;  // Up
    r->w = w;  // Width
    r->h = h;  // Height
}


/***************************  PRIVATE FUNCTIONS  ***************************/
/*
 * LoadSprite - Load the sprite associated to the id given, color keyed if
 *              required.
 */
static SDL_Surface *LoadSprite (const char *filename, int keyed)
{
    int e;
    SDL_Surface *original, *optimized;

    original = SDL_LoadBMP( MediaFile( filename ) ); 
    if( original == NULL )
        Fatal( "Loading sprite '%s'.", filename ); 

    // Optimize the surface for fast blitting
    optimized = SDL_DisplayFormat( original );
    SDL_FreeSurface( original );

    if( keyed ) {
        e = SDL_SetColorKey( optimized, SDL_SRCCOLORKEY | SDL_RLEACCEL,
                             m_ColorKey );
        if( e < 0 )
            Fatal( "Setting color key on '%s'.", filename );
    }

    return optimized;
}


/*
 * PaintRoundMessage
 */
static void PaintRoundMessage (void)
{
    int e, i, x, y;
    char num[4];
    SDL_Rect r1, r2;

    x = CENTER_WIDTH( FONT_RO_WIDTH ) + FONT_RO_WIDTH + 10;
    y = (SCREEN_HEIGHT/2)-(36/2);

    snprintf( num, 3, "%d", g_Round ); num[3] = '\0';

    SetRect( &r1, 0, FONT_HEIGHT, FONT_RO_WIDTH, FONT_HEIGHT );
    SetRect( &r2, CENTER_WIDTH( FONT_RO_WIDTH ), y, 0, 0 );
    e = SDL_BlitSurface( m_Font, &r1, m_Screen, &r2 );
    if( e < 0 )
        Warning( "Font not drawn." );

    for( i = 0; num[i] != '\0'; i++ ) {
        SetRect( &r1, 0, (FONT_HEIGHT * ((num[i] - '0') + FONT_ZERO)),
                 FONT_WIDTH, FONT_HEIGHT );
        SetRect( &r2, x, y, FONT_WIDTH, FONT_HEIGHT );
        e = SDL_BlitSurface( m_Font, &r1, m_Screen, &r2 );
        if( e < 0 )
            Warning( "Font not drawn." );
        x += FONT_WIDTH + 2;
    }
}


/*
 * PaintGameOverMessage
 */
static void PaintGameOverMessage (void)
{
    int e;
    SDL_Rect r1, r2;

    SetRect( &r1, 0, 0, FONT_GO_WIDTH, FONT_HEIGHT );
    SetRect( &r2, CENTER_WIDTH( FONT_GO_WIDTH ), CENTER_HEIGHT( FONT_HEIGHT ),
             0, 0 );
    e = SDL_BlitSurface( m_Font, &r1, m_Screen, &r2 );
    if( e < 0 )
        Warning( "Font not drawn." );
}


/****************************  PUBLIC FUNCTIONS  ****************************/
/*
 * InitGraphcis - Setups the screen settings. On error aborts.
 */
void InitGraphics (int fullscreen)
{
    id_t i;
    Uint32 screen_flags;
    
    screen_flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
    if( fullscreen )
        screen_flags |= SDL_FULLSCREEN;

    m_Screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH,
                                 screen_flags );
    if( m_Screen == NULL )
        Fatal( "Setting video mode." );

    m_ColorKey    = SDL_MapRGB( m_Screen->format, 0xFF, 0x00, 0xFF );
    m_ClearColor  = SDL_MapRGB( m_Screen->format, 0x00, 0x00, 0x00 );
    m_HealthColor = SDL_MapRGB( m_Screen->format, 0x00, 0x00, 0xFF );
    m_Background  = (SDL_Surface *) 0;

    // Clear the sprite array
    memset( m_Sprite, 0, ID_T_MAX * sizeof( SDL_Surface * ) );

    // And load the sprites
    m_Background = LoadSprite( "background.bmp", 0 );
    m_Splash     = LoadSprite( "splash.bmp",     0 );
    m_Font       = LoadSprite( "font.bmp",       0 );

    for( i = PLAYER; i < ID_T_MAX; i++ ) 
        m_Sprite[i] = LoadSprite( c_Metrics[i].filename, 1 );
}


/*
 * ClearScreen
 */
void ClearScreen (void)
{
    int e;
    SDL_Rect r1, r2;

    SetRect( &r1, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    SetRect( &r2, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

    switch( g_GameState ) {
        case INITIAL:
            e = SDL_BlitSurface( m_Splash, &r1, m_Screen, &r2 );
            break;
        case PLAYING:
            e = SDL_BlitSurface( m_Background, &r1, m_Screen, &r2 );
            break;
        case ROUND_CHANGE:
            e = SDL_FillRect( m_Screen, &r1, m_ClearColor );
            PaintRoundMessage();
            break;
        case GAME_OVER:
            e = SDL_FillRect( m_Screen, &r1, m_ClearColor );
            PaintGameOverMessage();
            break;
        default:
            e = 0;
    }
    if( e < 0 )
        Warning( "Screen not cleared." );
}


/*
 * FlushScreen
 */
void FlushScreen (void)
{
    int e;

    e = SDL_Flip( m_Screen );
    if( e < 0 )
        Warning( "Buffers not swapped." );
}


/*
 * DrawItem - Paints a drawable item onto the screen.
 */
void DrawItem (item_t *item)
{
    SDL_Rect src, dst;
    int e, w, h;
    int x = 0, y = 0;

    if( item->type >= PLAYER_ATTACK ) {
        switch( item-> action ) {
            case MOVING:
                x = (item->direction & WEST) + (item->frame & 0x01);
                break;
            case DYING:
                x = 4 + (item->frame & 0x03);
                break;
            default:
                return;
        }
    } else {
        switch( item->action ) {
            case MOVING:
                x = ((item->direction & WEST) << 1) + (item->frame & 0x03);
                y = item->direction & NORTH;
                break;
            case AIMING:
                x = (item->direction & WEST) >> 1;
                y = 2;
                break;
            case SHOOTING:
                x = 2 + ((item->direction & WEST) >> 1);
                y = 2;
                break;
            case HIT:
                x = 4 + ((item->direction & WEST) >> 1);
                y = 2;
                break;
            case DYING:
                x = item->frame & 0x07;
                y = 3;
                break;
            default:
                return;
        }
    }

    w = 1 << item->h_frame_shift;
    h = 1 << item->v_frame_shift;
    x = x << item->h_frame_shift;
    y = y << item->v_frame_shift;

    SetRect( &src, x, y, w, h );
    SetRect( &dst, item->x_pos, (item->z_pos - item->box_height), w, h );

    e = SDL_BlitSurface( m_Sprite[item->type], &src, m_Screen, &dst );
    if( e < 0 )
        Warning( "Sprite '%s' not drawn.", c_Metrics[item->type].filename );
}


/*
 * DrawHealth - Draws the player health status.
 */
void DrawHealth( item_t *player )
{
    int percent, e;
    SDL_Rect r;

    if( (g_GameState != PLAYING) || (player->strength <= 0) )
        return;

    // Get percent value (quite expensive, could be optimized)
    percent = (player->strength * 100) / c_Metrics[PLAYER].strength;

    SetRect( &r, HEALTH_XPOS, HEALTH_YPOS, percent, HEALTH_HEIGHT );
    e = SDL_FillRect( m_Screen, &r, m_HealthColor );
    if( e < 0 )
        Warning( "Health status not displayed." );
}


/*
 * CleanUpGraphics - Free all surfaces loaded.
 */
void CleanUpGraphics (void)
{
    id_t i;

    if( m_Background != NULL )
        SDL_FreeSurface( m_Background );

    if( m_Splash != NULL )
        SDL_FreeSurface( m_Splash );

    if( m_Font != NULL )
        SDL_FreeSurface( m_Font );

    for( i = 0; i < ID_T_MAX; i++ ) 
        if( m_Sprite[i] != (SDL_Surface *)0 )
            SDL_FreeSurface( m_Sprite[i] );
}

