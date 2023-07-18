#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "../include/defs.hh"
#include "../include/primitive.hh"
#include "../include/fragment.hh"

constexpr int MAX_DATA_SIZE = 32;

// Just helper function for "bool clipping_line(p1, p2)"
bool clipping_line_set_t0_t1(float p, float q, float &t0, float &t1)
{
    if(p == 0) return q >= 0;
    else if (p < 0) t0 = std::max(t0, q / p); // line goes into viewport
    else t1 = std::min(t1, q / p); // line goes to outside of viewport
        
    return true;
}

// Clipping of lines, by using Liang-Barsky Algorithm
// https://www.skytopia.com/project/articles/compsci/clipping.html
// https://www.geeksforgeeks.org/liang-barsky-algorithm/
bool clipping_line(VshaderOutput& p1, VshaderOutput& p2)
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
    && t0 < t1) 
    {
        p1.pos[0] = x1 + t0 * (x2 - x1); 
        p1.pos[1] = y1 + t0 * (y2 - y1); 
        p1.pos[2] = mmath::interpolate(z1, z2, 1 - t0, t0);
        p1.pos[3] = mmath::interpolate(w1, w2, 1 - t0, t0);
        p1.tp = t0;
        
        p2.pos[0] = x1 + t1 * (x2 - x1); 
        p2.pos[1] = y1 + t1 * (y2 - y1);
        p2.pos[2] = mmath::interpolate(z1, z2, 1 - t1, t1);
        p2.pos[3] = mmath::interpolate(w1, w2, 1 - t1, t1);
        p2.tp = t1;

        // perspective correct barycentry.
        float perp_corr_t0 = t0 * w2 / ((1 - t0) * w1 + t0 * w2);
        float perp_corr_t1 = t1 * w2 / ((1 - t1) * w1 + t1 * w2);

        for(int i = 0; i < context.vshader_out_data_size; i++)
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
void draw_line_with_dda(const VshaderOutput& p1,
    const VshaderOutput& p2, std::vector<ShadedFragment>* fragments)
{
    int dx = p2.pos.x - p1.pos.x;
    int dy = p2.pos.y - p1.pos.y;
    float steps = (float)std::max(abs(dx), abs(dy));

    float x_inc = dx / (float) steps;
    float y_inc = dy / (float) steps;

    float x = p1.pos.x, y = p1.pos.y;

    for(int i = 0; i <= steps; i++, x += x_inc, y += y_inc)
    {
        Fragment one;
        one.f_data = context.fshader_out_data_buf;

        const float t = i / (float)steps;

        // barycentric coordinate in clip space(namely, world space)
        const float correct_barycentric = (1 - t) * p1.pos.w / 
            ((1 - t) * p1.pos.w + t * p2.pos.w);

        for(int j = 0; j <= context.vshader_out_data_size; j++)
            one.f_data[j] = mmath::interpolate(p1.data[j], p2.data[j],
                correct_barycentric, 1 - correct_barycentric);
            
        one.x = round(x), one.y = round(y);
        one.z = mmath::interpolate(p1.pos.z, p2.pos.z, 1 - t, t);

        if(0 > one.x || one.x >= context.window_w) continue;
        if(0 > one.y || one.y >= context.window_h) continue;
        if(0 > one.z || one.z > 1) continue;

        fragments->push_back(call_fragment_shader(&one));
    }
}

// Draw Triangle
void draw_triangle(const VshaderOutput& p1, const VshaderOutput& p2,
    const VshaderOutput& p3, std::vector<ShadedFragment>* fragments)
{
    const int xmin = std::max(0, (int)std::min({p1.pos.x, p2.pos.x, p3.pos.x}));
    const int xmax = std::min(context.window_w - 1,
        (int)std::max({p1.pos.x, p2.pos.x, p3.pos.x}));
    const int ymin = std::max(0, (int)std::min({p1.pos.y, p2.pos.y, p3.pos.y}));
    const int ymax = std::min(context.window_h - 1,
        (int)std::max({p1.pos.y, p2.pos.y, p3.pos.y}));

    // the coordinate of vertex, in window coordinate system.
    const mmath::Vec2<float> w_pos1 = {p1.pos.x, p1.pos.y};
    const mmath::Vec2<float> w_pos2 = {p2.pos.x, p2.pos.y};
    const mmath::Vec2<float> w_pos3 = {p3.pos.x, p3.pos.y};

    for(int x = xmin; x <= xmax; x++)
    {
        for(int y = ymin; y <= ymax; y++)
        {
            mmath::Vec2<float> fragment_pos = {x, y};
            float t1 = -mmath::cross(w_pos3 - w_pos2, fragment_pos - w_pos2);
            float t2 = -mmath::cross(w_pos1 - w_pos3, fragment_pos - w_pos3);
            float t3 = -mmath::cross(w_pos2 - w_pos1, fragment_pos - w_pos1);

            if(t1 < 0 || t2 < 0 || t3 < 0) continue;
                        
            // barycentric coordinate in window coordinate system.
            auto w_bary = mmath::Vec3<float>(t1, t2, t3) / (t1 + t2 + t3);
            
            // barycentric coordinate in clip space(namely, world space)
            auto perp_corr_bary = mmath::Vec3<float>(
                w_bary.x * p1.pos.w, w_bary.y * p2.pos.w, w_bary.z * p3.pos.w) /
                (w_bary.x * p1.pos.w + w_bary.y * p2.pos.w + w_bary.z * p3.pos.w);
            
            Fragment one;
            one.f_data = context.fshader_out_data_buf;
            for(int j = 0; j <= context.vshader_out_data_size; j++)
            {
                one.f_data[j] = mmath::interpolate(
                    p1.data[j], p2.data[j], p3.data[j], perp_corr_bary);
            }
            one.x = x, one.y = y;
            one.z = mmath::interpolate(p1.pos.z, p2.pos.z, p3.pos.z, w_bary);

            if(0 <= one.z && one.z <= 1)
                fragments->push_back(call_fragment_shader(&one));
        }
        
    }
}