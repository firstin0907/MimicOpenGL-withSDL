#include "../include/defs.hh"
#include "../include/mimicMath.hh"
#include "../include/mimic.hh"

#include <cmath>
#include <iostream>
float data[30];
int data_cnt;

void set_vertex_shader(void (*vertex_shader)(float*[], mmath::Vec4<float>*, float[]))
{
    context.vertex_shader = vertex_shader;
}

VertexShaderOutput call_vertex_shader(int vertex_number)
{
    data_cnt += 4;
    if(data_cnt >= 20) data_cnt = 0;
    auto vao_table = context.binded_vao->vao_table;

    mmath::Vec4<float> pos;
    float* vbo_pointer[MAX_VERTEX_ATTRIBS];
    for(int i = 0; i < MAX_VERTEX_ATTRIBS; i++)
    {
        vbo_pointer[i] = vao_table[i].pointer + vao_table[i].stride * vertex_number;
    }

    context.vertex_shader(vbo_pointer, &pos, data + data_cnt);
    // naive
    if(pos[3] < 0.001f) {
        pos[3] = 0.001f;
    }
    
    mmath::Mat4x4<float> Tvp =
    {
        context.window_w / 2.0f, 0, 0, context.window_w / 2.0f,
        0, -context.window_h / 2.0f, 0, context.window_h / 2.0f,
        0, 0, 0.5f, 0.5f,
        0, 0, 0, 1.0f
    };
    pos = Tvp * pos;
    
    pos[3] = 1 / pos[3];
    pos[0] *= pos[3];
    pos[1] *= pos[3];
    pos[2] *= pos[3];

    return {pos, data + data_cnt};
}