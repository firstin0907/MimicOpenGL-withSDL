#include "../include/defs.hh"
#include "../include/fragment.hh"
#include <algorithm>
#include <iostream>

void call_fragment_shader(Fragment* fragment)
{
    // call fragment shader

    // temporal
    int r = (int)std::min(fragment->f_data[0] * 255, 255.0f);
    int g = (int)std::min(fragment->f_data[1] * 255, 255.0f);
    int b = (int)std::min(fragment->f_data[2] * 255, 255.0f);

    fragment->color = (r << 24) | (g << 16) | (b << 8);
}