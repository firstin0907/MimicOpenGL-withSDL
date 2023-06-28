#include <iostream>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>

#include "../include/defs.hh"

typedef int color_t;

float z_buffer[1920 * 1080];
int color_buffer[1920 * 1080];

void appendFragment(Fragment* fragment, int pitch)
{
    const int index = fragment->x * pitch + fragment->y;
    if(z_buffer[index] > fragment->z)
    {
        z_buffer[index] = fragment->z;
        color_buffer[index] = fragment->color;
    }
}


int perSampleOperation(Context* context, std::vector<Fragment>* fragments)
{
    int* texture_pixels;
    int pitch;

    std::fill(z_buffer, z_buffer + sizeof(z_buffer) / sizeof(float), 100.0);
    std::fill(color_buffer, color_buffer + sizeof(color_buffer) / sizeof(int),
        0x333333FF);

    
    if(SDL_LockTexture(context->texture, NULL, (void **)&texture_pixels, &pitch) < 0)
    {
        return -1;
    }
    
    for(auto &fr : *fragments)
    {
        appendFragment(&fr, context->window_w);
    }

    for(int i = 0; i < context->window_h * context->window_w; i++)
    {
        texture_pixels[i] = color_buffer[i];
    } 

    SDL_UnlockTexture(context->texture);
    
    
    return 0;
}