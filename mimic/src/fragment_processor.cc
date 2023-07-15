#include "../include/defs.hh"
#include "../include/fragment.hh"
#include <algorithm>
#include <iostream>

ShadedFragment call_fragment_shader(Fragment* fragment)
{
    ShadedFragment shaded;
    // call fragment shader
    shaded.color = context.fragment_shader(fragment);
    shaded.x = fragment->x;
    shaded.y = fragment->y;
    shaded.z = fragment->z;
    return shaded;
}