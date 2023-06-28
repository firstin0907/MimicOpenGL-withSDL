#include "../include/defs.hh"
#include "../include/mimicMath.hh"
#include "../include/mimic.hh"

#include <iostream>
float data[30];
int data_cnt;

void set_vertex_shader(void (*vertex_shader)(float*[], mmath::Vec3<int>*, float[]))
{
    context.vertex_shader = vertex_shader;
}

PPoint call_vertex_shader(int vertex_number)
{
    data_cnt += 4;
    if(data_cnt >= 20) data_cnt = 0;
    auto vao_table = context.binded_vao->vao_table;

    mmath::Vec3<int> pos;
    float* vbo_pointer[MAX_VERTEX_ATTRIBS];
    for(int i = 0; i < MAX_VERTEX_ATTRIBS; i++)
    {
        vbo_pointer[i] = vao_table[i].pointer + vao_table[i].stride * vertex_number;
    }
    
    context.vertex_shader(vbo_pointer, &pos, data + data_cnt);
    return PPoint{pos.x, pos.y, pos.z, data + data_cnt};

    float x = *(vao_table[0].pointer + vao_table[0].stride * vertex_number);
    float y = *(vao_table[0].pointer + vao_table[0].stride * vertex_number + 1);
    float z = *(vao_table[0].pointer + vao_table[0].stride * vertex_number + 2);

    float r = *(vao_table[1].pointer + vao_table[1].stride * vertex_number);
    float g = *(vao_table[1].pointer + vao_table[1].stride * vertex_number + 1);
    float b = *(vao_table[1].pointer + vao_table[1].stride * vertex_number + 2);

    data[data_cnt + 0] = r, data[data_cnt + 1] = g, data[data_cnt + 2] = b;

    PPoint p1 = {x, y, z, data + data_cnt};
    data_cnt += 4;
    return p1;
}