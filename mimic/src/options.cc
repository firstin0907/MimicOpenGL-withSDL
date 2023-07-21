#include "../include/defs.hh"

void set_point_radius(int radius)
{
    context.drawing_options.point_radius = radius;     
}

void set_z_mode(bool t)
{
    context.drawing_options.z_mode = t;
}
