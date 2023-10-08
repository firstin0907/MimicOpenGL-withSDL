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

int perSampleOperation(Context* context, ShadedFragment* fragment)
{
    const int index = fragment->x + fragment->y * context->window_w;
    if(context->z_buffer[index] > fragment->z)
    {
        context->z_buffer[index] = fragment->z;
        
        if(context->drawing_options.z_mode)
            context->color_buffer[index] = (255 - (int)((fragment->z) * 255)) << 24;
        else context->color_buffer[index] = fragment->color;
    }
}

