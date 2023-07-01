#pragma once
#include "mimicMath.hh"
#define DRAW_POINTS     0
#define DRAW_LINES      1
#define DRAW_LINE_STRIP 2
#define DRAW_LINE_LOOP  3
#define DRAW_TRIANGLES  4

// mimic.cc
extern int StartMimicGL(int window_w, int window_h);
extern void DrawArrays(int start, int number, int type);
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




