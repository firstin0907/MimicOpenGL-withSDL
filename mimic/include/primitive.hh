#include <SDL2/SDL.h>
#include <stdint.h>

struct PPoint
{
    int32_t x, y;
};

struct PLine
{
    PPoint p1, p2;
};

struct PTriangle
{
    PPoint p1, p2, p3;
};