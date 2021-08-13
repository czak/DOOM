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

int xlatekey(SDL_Keysym *key)
{
    int rc;

    switch(key->sym)
    {
        case SDLK_LEFT:     rc = KEY_LEFTARROW;     break;
        case SDLK_RIGHT:    rc = KEY_RIGHTARROW;    break;
        case SDLK_DOWN:     rc = KEY_DOWNARROW;     break;
        case SDLK_UP:       rc = KEY_UPARROW;       break;
        case SDLK_ESCAPE:   rc = KEY_ESCAPE;        break;
        case SDLK_RETURN:   rc = KEY_ENTER;         break;
        case SDLK_TAB:      rc = KEY_TAB;           break;
        case SDLK_F1:       rc = KEY_F1;            break;
        case SDLK_F2:       rc = KEY_F2;            break;
        case SDLK_F3:       rc = KEY_F3;            break;
        case SDLK_F4:       rc = KEY_F4;            break;
        case SDLK_F5:       rc = KEY_F5;            break;
        case SDLK_F6:       rc = KEY_F6;            break;
        case SDLK_F7:       rc = KEY_F7;            break;
        case SDLK_F8:       rc = KEY_F8;            break;
        case SDLK_F9:       rc = KEY_F9;            break;
        case SDLK_F10:      rc = KEY_F10;           break;
        case SDLK_F11:      rc = KEY_F11;           break;
        case SDLK_F12:      rc = KEY_F12;           break;
        case SDLK_PAUSE:    rc = KEY_PAUSE;         break;
        case SDLK_EQUALS:   rc = KEY_EQUALS;        break;

        case SDLK_BACKSPACE:
        case SDLK_DELETE:   rc = KEY_BACKSPACE;     break;

        case SDLK_KP_MINUS:
        case SDLK_MINUS:    rc = KEY_MINUS;         break;

        case SDLK_LSHIFT:
        case SDLK_RSHIFT:   rc = KEY_RSHIFT;        break;

        case SDLK_LCTRL:
        case SDLK_RCTRL:    rc = KEY_RCTRL;         break;

        case SDLK_LALT:
        case SDLK_RALT:     rc = KEY_RALT;          break;

        default:            rc = key->sym;          break;
    }

    return rc;
}

void I_GetEvent(SDL_Event *sdl_event)
{
    event_t event;

    switch (sdl_event->type) {
        case SDL_KEYDOWN:
            event.type = ev_keydown;
            event.data1 = xlatekey(&sdl_event->key.keysym);
            D_PostEvent(&event);
            break;

        case SDL_KEYUP:
            event.type = ev_keyup;
            event.data1 = xlatekey(&sdl_event->key.keysym);
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
