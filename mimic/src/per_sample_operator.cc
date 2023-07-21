#include <iostream>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>

#include "../include/defs.hh"

typedef int color_t;

float z_buffer[1920 * 1080];
int color_buffer[1920 * 1080];

void appendFragment(ShadedFragment* fragment, int pitch)
{
    const int index = fragment->x + fragment->y * pitch;
    if(z_buffer[index] > fragment->z)
    {
        z_buffer[index] = fragment->z;
        
        if(context.drawing_options.z_mode)
            color_buffer[index] = (255 - (int)((fragment->z) * 255)) << 24;
        else color_buffer[index] = fragment->color;
    }
}


int perSampleOperation(Context* context, std::vector<ShadedFragment>* fragments)
{
    int* texture_pixels;
    int pitch;

    std::fill(z_buffer, z_buffer + sizeof(z_buffer) / sizeof(float), 100.0);
    std::fill(color_buffer, color_buffer + sizeof(color_buffer) / sizeof(int),
        context->drawing_options.clear_color);


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