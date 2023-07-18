#pragma once

typedef int color_t;

struct Fragment
{
    int x, y;
    float z;
    float*  f_data; // interpolated input data
};

struct ShadedFragment
{
    int x, y;
    float z;
    color_t color;
};