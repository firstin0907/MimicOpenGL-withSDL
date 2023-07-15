#include "../include/defs.hh"
#include "../include/mimicMath.hh"
#include "../include/mimic.hh"

#include <cmath>
#include <iostream>

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
    // naive
    if(pos[3] < 0.00001)  pos[3] = 0.00001;
    
    mmath::Mat4x4<double> Tvp =
    {
        context.window_w / 2.0, 0, 0, context.window_w / 2.0,
        0, -context.window_h / 2.0, 0, context.window_h / 2.0,
        0, 0, 0.5, 0.5,
        0, 0, 0, 1.0
    };
    pos = Tvp * pos;
    
    pos[3] = 1 / pos[3];
    pos[0] *= pos[3];
    pos[1] *= pos[3];
    pos[2] *= pos[3];

    return {pos, context.vshader_out_data_buf + data_cnt};
}