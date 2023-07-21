#include "../include/defs.hh"

void set_point_radius(int radius)
{
    context.drawing_options.point_radius = radius;     
}

void set_z_mode(bool t)
{
    context.drawing_options.z_mode = t;
}

void set_clear_color(color_t color)
{
    context.drawing_options.clear_color = color;
}