#include <iostream>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>

#include "../include/defs.hh"

typedef int color_t;

void appendFragment(float* z_buffer, color_t* color_buffer,
    ShadedFragment* fragment, int pitch, bool z_mode)
{
    const int index = fragment->x + fragment->y * pitch;
    if(z_buffer[index] > fragment->z)
    {
        z_buffer[index] = fragment->z;
        
        if(z_mode)
            color_buffer[index] = (255 - (int)((fragment->z) * 255)) << 24;
        else color_buffer[index] = fragment->color;
    }
}


int perSampleOperation(Context* context, std::vector<ShadedFragment>* fragments)
{
    int* texture_pixels;
    int pitch;

    std::fill(context->z_buffer, context->z_buffer + 1920 * 1080, 100.0);
    std::fill(context->color_buffer, context->color_buffer + 1920 * 1080,
        context->drawing_options.clear_color);


    if(SDL_LockTexture(context->texture, NULL, (void **)&texture_pixels, &pitch) < 0)
    {
        return -1;
    }
    
    for(auto &fr : *fragments)
    {
        appendFragment(context->z_buffer, context->color_buffer,
            &fr, context->window_w, context->drawing_options.z_mode);
    }

    for(int i = 0; i < context->window_h * context->window_w; i++)
    {
        texture_pixels[i] = context->color_buffer[i];
    } 

    SDL_UnlockTexture(context->texture);
    
    
    return 0;
}