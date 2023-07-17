#include "../include/defs.hh"
#include "../include/mimicMath.hh"
#include "../include/mimic.hh"

#include <cmath>
#include <iostream>

inline void clip_space_to_viewport_space(mmath::Vec4<double>& clip_pos)
{
    // pre-calculate w-inverse.
    // (i.e. z-inverse with respect to view space)
    double w_inv = 1 / clip_pos.w;

    mmath::Mat4x4<double> Tvp =
    {
        context.window_w / 2.0, 0, 0, context.window_w / 2.0,
        0, -context.window_h / 2.0, 0, context.window_h / 2.0,
        0, 0, 0.5, 0.5,
        0, 0, 0, 1.0
    };
    
    // perspective division(divide by w)
    clip_pos = clip_pos * w_inv;

    // map point in NDC space into point viewport space
    clip_pos = Tvp * clip_pos;

    // store w-inverse in w coordinate, instead of 1, in order to use it
    // for calculation of perspective correct varicentric coordinate.
    clip_pos.w = w_inv;
}

int data_cnt = 0;
VshaderOutput call_vertex_shader(int vertex_number)
{
    data_cnt += context.vshader_out_data_size;
    if(data_cnt >= context.vshader_out_data_size * 4) data_cnt = 0;
    
    auto vao_table = context.binded_vao->vao_table;

    mmath::Vec4<double> pos;
    double* vbo_pointer[MAX_VERTEX_ATTRIBS];
    for(int i = 0; i < MAX_VERTEX_ATTRIBS; i++)
    {
        vbo_pointer[i] = vao_table[i].pointer + vao_table[i].stride * vertex_number;
    }

    context.vertex_shader(vbo_pointer, &pos, context.vshader_out_data_buf + data_cnt);

    return {pos, context.vshader_out_data_buf + data_cnt};
}

bool vertex_post_processing_for_lines(VshaderOutput &p1, VshaderOutput& p2)
{
    auto clip_func = [](double p, double q, double &t0, double &t1)
    {
        if(p == 0) return q >= 0;
        else if (p < 0) t0 = std::max(t0, q / p); // line goes into viewport
        else t1 = std::min(t1, q / p); // line goes to outside of viewport
            
        return true;
    };

    // Primitive Clipping
    const double x1 = p1.pos[0], x2 = p2.pos[0];
    const double y1 = p1.pos[1], y2 = p2.pos[1];
    const double z1 = p1.pos[2], z2 = p2.pos[2];
    const double w1 = p1.pos[3], w2 = p2.pos[3];

    double t0 = 0.0, t1 = 1.0;
    if(clip_func((x1 + w1) - (x2 + w2), (x1 + w1) - 0, t0, t1)
    && clip_func((x2 - w2) - (x1 - w1), 0 - (x1 - w1), t0, t1)
    && clip_func((y1 + w1) - (y2 + w2), (y1 + w1) - 0, t0, t1)
    && clip_func((y2 - w2) - (y1 - w1), 0 - (y1 - w1), t0, t1)
    && clip_func((z1 + w1) - (z2 + w2), (z1 + w1) - 0, t0, t1)
    && clip_func((z2 - w2) - (z1 - w1), 0 - (z1 - w1), t0, t1)
    && t0 < t1)
    {
        p1.pos[0] = x1 + t0 * (x2 - x1); 
        p1.pos[1] = y1 + t0 * (y2 - y1); 
        p1.pos[2] = mmath::interpolate(z1, z2, 1 - t0, t0);
        p1.pos[3] = mmath::interpolate(w1, w2, 1 - t0, t0);
        p1.tp = t0;
        
        p2.pos[0] = mmath::interpolate(x1, x2, 1 - t1, t1);
        p2.pos[1] = mmath::interpolate(y1, y2, 1 - t1, t1);
        p2.pos[2] = mmath::interpolate(z1, z2, 1 - t1, t1);
        p2.pos[3] = mmath::interpolate(w1, w2, 1 - t1, t1);
        p2.tp = t1;

        for(int i = 0; i < context.vshader_out_data_size; i++)
        {
            const double attr1 = p1.data[i], attr2 = p2.data[i];
            
            p1.data[i] = mmath::interpolate(attr1, attr2, 1 - t0, t0);
            p2.data[i] = mmath::interpolate(attr1, attr2, 1 - t1, t1);
        }
        
        // Persepective Division and Viewport trasformation
        clip_space_to_viewport_space(p1.pos);
        clip_space_to_viewport_space(p2.pos);

        return true;
    }
    return false;
}   

bool vertex_post_processing_for_points(VshaderOutput &p)
{
    
    clip_space_to_viewport_space(p.pos);

    return true;
}   