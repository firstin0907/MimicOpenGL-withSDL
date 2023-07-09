#pragma once
#include <stdint.h>
#include "mimicMath.hh"

enum DrawingType
{
    DRAW_POINTS,
    DRAW_LINES,
    DRAW_LINE_STRIP,
    DRAW_LINE_LOOP,
    DRAW_TRIANGLES,
    DRAW_TRIANGLES_FRAME
};

// mimic.cc
extern int StartMimicGL(int window_w, int window_h);
extern void DrawArrays(const uint32_t start, const uint32_t number,
    DrawingType type);
extern int DrawFrame();
extern int TerminateMimicGL();

// vao_vbo.cc
extern void bindVertexArray(struct VAO* vao);
extern void bindBuffer(struct VBO* vbo);
extern void copyIntoBufferData(int size, float* data);
extern void setVaoPointer(int index, int size, int stride, int offset);
extern struct VAO* generateVertexArray();
extern struct VBO* generateBuffer();
extern struct VBO* deleteBuffer(struct VBO* vbo);

// vertex_processor.cc
extern void set_vertex_shader(void (*vertex_shader)(float*[], mmath::Vec4<float>*, float[]));




