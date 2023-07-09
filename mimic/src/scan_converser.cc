#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "../include/defs.hh"
#include "../include/primitive.hh"
#include "../include/fragment.hh"

constexpr int MAX_DATA_SIZE = 32;

float data_buffer[MAX_DATA_SIZE];

// Just helper function for "bool clipping_line(p1, p2)"
bool clipping_line_set_t0_t1(float p, float q, float &t0, float &t1)
{
    if(p == 0)
    {
        if(q < 0) return false;
    }
    else if (p < 0) t0 = std::max(t0, q / p); // line goes into viewport
    else t1 = std::min(t1, q / p); // line goes to outside of viewport
        
    return true;
}

// Clipping of lines, by using Liang-Barsky Algorithm
// https://www.skytopia.com/project/articles/compsci/clipping.html
// https://www.geeksforgeeks.org/liang-barsky-algorithm/
bool clipping_line(VertexShaderOutput& p1, VertexShaderOutput& p2, int data_size)
{
    const float x1 = p1.pos[0], x2 = p2.pos[0];
    const float y1 = p1.pos[1], y2 = p2.pos[1];
    const float z1 = p1.pos[2], z2 = p2.pos[2];
    const float w1 = p1.pos[3], w2 = p2.pos[3];

    float t0 = 0.0, t1 = 1.0;
    if(clipping_line_set_t0_t1(x1 - x2, x1 - 0, t0, t1)
    && clipping_line_set_t0_t1(x2 - x1, context.window_w - x1, t0, t1)
    && clipping_line_set_t0_t1(y1 - y2, y1 - 0, t0, t1)
    && clipping_line_set_t0_t1(y2 - y1, context.window_h - y1, t0, t1)
    && clipping_line_set_t0_t1(z1 - z2, z1 - 0, t0, t1)
    && clipping_line_set_t0_t1(z2 - z1, 1 - z1, t0, t1)
    && t0 < t1)
    {
        p1.pos[0] = x1 + t0 * (x2 - x1); 
        p1.pos[1] = y1 + t0 * (y2 - y1); 
        p2.pos[2] = mmath::interpolate(p1.pos[2], p2.pos[2], 1 - t0, t0);
        p1.pos[3] = mmath::interpolate(p1.pos[3], p2.pos[3], 1 - t0, t0);
        
        p2.pos[0] = x1 + t1 * (x2 - x1); 
        p2.pos[1] = y1 + t1 * (y2 - y1);
        p2.pos[2] = mmath::interpolate(p1.pos[2], p2.pos[2], 1 - t1, t1);
        p2.pos[3] = mmath::interpolate(p1.pos[3], p2.pos[3], 1 - t1, t1);
    
        // perspective correct barycentry.
        float perp_corr_t0 = t0 * w2 / ((1 - t0) * w1 + t0 * w2);
        float perp_corr_t1 = t1 * w2 / ((1 - t1) * w1 + t1 * w2);
        
        for(int i = 0; i < data_size; i++)
        {
            const float attr1 = p1.data[i], attr2 = p2.data[i];
            
            p1.data[i] = mmath::interpolate(attr1, attr2, (1 - perp_corr_t0), perp_corr_t0);
            p2.data[i] = mmath::interpolate(attr1, attr2, (1 - perp_corr_t1), perp_corr_t1);
        }
        return true;
    }
    return false;
}

// Draw Line by using DDA Algorithm
// https://www.tutorialspoint.com/computer_graphics/line_generation_algorithm.htm
void draw_line_with_dda(const VertexShaderOutput& p1,
    const VertexShaderOutput& p2, int data_length,
    std::vector<Fragment>* fragments)
{
    int dx = p1.pos[0] - p2.pos[0];
    int dy = p1.pos[1] - p2.pos[1];
    double steps = (double)std::max(abs(dx), abs(dy));

    double x_inc = dx / (double) steps;
    double y_inc = dy / (double) steps;

    double x = p2.pos[0], y = p2.pos[1];

    for(int i = 0; i <= steps; i++)
    {
        Fragment one;

        const float t0 = i / steps;
        const float t1 = 1 - t0;
        
        float perp_corr_t0 = t0 * p1.pos[3] / (t0 * p1.pos[3] + t1 * p2.pos[3]);
        float perp_corr_t1 = t1 * p2.pos[3] / (t0 * p1.pos[3] + t1 * p2.pos[3]);
        
        one.f_data = data_buffer;
        for(int j = 0; j <= data_length; j++)
        {
            one.f_data[j] = perp_corr_t0 * p1.data[j]
                + perp_corr_t1 * p2.data[j];
        }
        one.x = round(x), one.y = round(y);
        one.z = mmath::interpolate(p1.pos[2], p2.pos[2], t0, t1);

        x += x_inc;
        y += y_inc;

        if(one.x < 0 || one.y < 0) continue;
        if(one.x > context.window_w || one.y > context.window_h) continue;

        call_fragment_shader(&one);
        
        fragments->push_back(one);
    }
}

// Draw Triangle
void draw_triangle(PTriangle triangle, int data_length,
    std::vector<Fragment>* fragments)
{
    
}