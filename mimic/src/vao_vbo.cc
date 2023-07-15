#include "../include/defs.hh"

// glVBindVertextArray in OpenGL
void bindVertexArray(VAO* vao)
{
    context.binded_vao = vao;
}

// glBindBuffer in OpenGL
void bindBuffer(VBO* vbo)
{
    context.binded_vbo = vbo;
}

// glBufferData in OpenGL
void copyIntoBufferData(int size, double* data)
{
    context.binded_vbo->object = new double[size];
    for(int i = 0; i < size; i++)
    {
        context.binded_vbo->object[i] = data[i];
    }
}

// glVertexAttribPointer in OpenGL
// Must be called after buffer data copied (by copyIntoBufferData function).
void setVaoPointer(int index, int size, int stride, int offset)
{
    auto& entry = context.binded_vao->vao_table[index];
    entry.index = index;
    entry.size = size;
    entry.stride = stride;
    entry.pointer = context.binded_vbo->object + offset;
}

// Create VAO and returns created VAO's address.
VAO* generateVertexArray()
{
    VAO* vao = new VAO();
    context.vaos.push_back(vao);
    return vao;
}

// Create VBO and returns created VBO's address.
VBO* generateBuffer()
{
    VBO* vbo = new VBO();
    context.vbos.push_back(vbo);
    return vbo;
}

VBO* deleteBuffer(VBO* vbo)
{
    delete[] vbo->object;
    delete vbo;
}