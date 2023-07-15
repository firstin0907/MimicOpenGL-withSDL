#pragma once
#include <stdint.h>
#include "mimicMath.hh"

typedef int color_t;

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
extern void copyIntoBufferData(int size, double* data);
extern void setVaoPointer(int index, int size, int stride, int offset);
extern struct VAO* generateVertexArray();
extern struct VBO* generateBuffer();
extern struct VBO* deleteBuffer(struct VBO* vbo);

/**
 * Set vertex shader and fragment shader to be used for
 * vertex processing and fragment processing.
 * 
 * \param out_size: The size of out variables produced from vertex_shader.
 *                  Namely, the size of variables to be inputted to fragment
 *                  shader. All of out variables should be double,
 *                  so the total bytes of out variable will be (8 * out_size).
 * \param vertex_shader: The shader function to be used for vertex processing.
 *                       This function takes the attributes by first argument,
 *                       and should return the position(2nd argument)
 *                       and the out variables(3rd argument).
 * \param fragment_shader: The shader function to be used for fragment
 *                         processing. This function only takes one argument,
 *                         fragment_shader, which contains 3D position in
 *                         viewport space and in variables data.
 */
extern void set_shaders(
    const uint32_t out_size,
    void (*vertex_shader)(double*[], mmath::Vec4<double>*, double[]),
    color_t (*fragment_shader)(struct Fragment*)
);

extern void set_z_mode(bool t);

