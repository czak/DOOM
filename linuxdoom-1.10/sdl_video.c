//-----------------------------------------------------------------------------
//
//	DOOM graphics stuff for SDL2
//
//-----------------------------------------------------------------------------

#include <SDL2/SDL.h>

#include "doomdef.h"
#include "v_video.h"
#include "d_main.h"
#include "i_system.h"

SDL_Window *window;
SDL_Surface *backbuffer;
SDL_Surface *screen;

void I_InitGraphics(void)
{
    window = SDL_CreateWindow("DOOM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 200, 0);
    backbuffer = SDL_CreateRGBSurfaceFrom(screens[0], 320, 200, 8, 320, 0, 0, 0, 0);
    screen = SDL_GetWindowSurface(window);
}

void I_ShutdownGraphics(void)
{
    SDL_FreeSurface(backbuffer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void I_StartFrame(void)
{
}

void I_UpdateNoBlit(void)
{
}

void I_ReadScreen(byte* scr)
{
    memcpy(scr, screens[0], SCREENWIDTH * SCREENHEIGHT);
}

void I_SetPalette(byte* palette)
{
    SDL_Color colors[256];

    for (int i=0; i<256; ++i) {
        colors[i].r = gammatable[usegamma][*palette++];
        colors[i].g = gammatable[usegamma][*palette++];
        colors[i].b = gammatable[usegamma][*palette++];
        colors[i].a = 0xff;
    }

    SDL_SetPaletteColors(backbuffer->format->palette, colors, 0, 256);
}

void I_FinishUpdate(void)
{
    SDL_BlitSurface(backbuffer, NULL, screen, NULL);
    SDL_UpdateWindowSurface(window);
}

void I_GetEvent(SDL_Event *sdl_event)
{
    event_t event;

    switch (sdl_event->type) {
        case SDL_KEYDOWN:
            event.type = ev_keydown;
            event.data1 = KEY_ENTER;
            D_PostEvent(&event);
            break;

        case SDL_QUIT:
            I_Quit();
    }
}

void I_StartTic(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
        I_GetEvent(&event);
}
