#include <SDL2/SDL.h>
#include "../include/mimic.hh"

Context context;

int StartMimicGL(int window_w, int window_h)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    context.window = SDL_CreateWindow(
        "Hello World", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, window_w, window_h, SDL_WINDOW_ALLOW_HIGHDPI
    );

    if(context.window == nullptr) return -1;
    return 0;    
}

int DrawFrame()
{
    return 0;
}

int TerminateMimicGL()
{
    SDL_DestroyWindow(context.window);
    SDL_Quit();
    return 0;
}