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

PPoint call_vertex_shader(int vertex_number)
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
    
    if(pos[3] < 0.001f) pos = {0, 0, 0, 1};
    pos = pos / pos[3];
    
    mmath::Mat4x4<float> Tvp =
    {
        context.window_w / 2.0f, 0, 0, context.window_w / 2.0f,
        0, -context.window_h / 2.0f, 0, context.window_h / 2.0f,
        0, 0, 0.5f, 0.5f,
        0, 0, 0, 1.0f
    };
    
    pos = Tvp * mmath::Vec4<float>(pos, 1.0f);

    return PPoint{(int)round(pos[0]), (int)round(pos[1]), (int)round(pos[2]), data + data_cnt};
}