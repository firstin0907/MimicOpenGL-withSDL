#pragma once

typedef int color_t;

struct Fragment
{
    int x, y, z;
    color_t color;

    int*    i_data; // interpolated
    float*  f_data;
};