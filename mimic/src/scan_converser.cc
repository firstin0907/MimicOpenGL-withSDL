#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "../include/defs.hh"
#include "../include/primitive.hh"
#include "../include/fragment.hh"

constexpr int MAX_DATA_SIZE = 32;

float data_buffer[MAX_DATA_SIZE];

// Draw Line by using DDA Algorithm
// https://www.tutorialspoint.com/computer_graphics/line_generation_algorithm.htm
void draw_line_with_dda(PLine line, int data_length,
    std::vector<Fragment>* fragments)
{
    int dx = line.p1.x - line.p2.x;
    int dy = line.p1.y - line.p2.y;
    double steps = (double)std::max(abs(dx), abs(dy));

    double x_inc = dx / (double) steps;
    double y_inc = dy / (double) steps;

    double x = line.p2.x, y = line.p2.y;
    
    for(int i = 0; i <= steps; i++)
    {
        const float p1_potion = i / steps;
        const float p2_potion = 1 - p1_potion;
        
        Fragment one;
        one.f_data = data_buffer;
        for(int j = 0; j <= data_length; j++)
        {
            one.f_data[j] = p1_potion * line.p1.out_data[j]
                + p2_potion * line.p2.out_data[j];
        }
        one.x = x, one.y = y;
        one.z = p1_potion * line.p1.z + p2_potion * line.p2.z;

        call_fragment_shader(&one);
        
        fragments->push_back(one);
        x += x_inc;
        y += y_inc;
    }
}

// Draw Triangle
void draw_triangle(PTriangle triangle, int data_length,
    std::vector<Fragment>* fragments)
{
    
}