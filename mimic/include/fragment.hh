#pragma once

typedef int color_t;

struct Fragment
{
    int x, y;
    double z;
    double*  f_data; // interpolated input data
};

struct ShadedFragment
{
    int x, y;
    double z;
    color_t color;
};