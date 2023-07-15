#pragma once

#include <stdint.h>

struct PPoint
{
    int32_t x, y, z;
    double* out_data;
};

struct PLine
{
    PPoint p1, p2;
};

struct PTriangle
{
    PPoint p1, p2, p3;
};