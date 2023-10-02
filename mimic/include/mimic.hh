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
/**
 * Conducts initializing.
 * Must be called before any other functions of this file is called.
 * 
 * \param window_w: The width of the window which is going to show
 *                  the rendered result.
 * \param window_h: The width of the window which is going to show
 *                  the rendered result.
 * \returns 0 if success, -1 if error.
*/
extern int StartMimicGL(int window_w, int window_h);

/**
 * Draws the object based on VAO and VBO binded by bindVertexArray() and
 * bindBuffer() call. The result will not be shown until the user calls
 * DrawFrame() function.
 * 
 * \param start: The index of VBO to be started to be drawn. 
 * \param number: The number of vertex to be drawn.
 * \param type: Specifies the method how to draw.
*/
extern void DrawArrays(const uint32_t start, const uint32_t number,
    DrawingType type);

/**
 * Update the window, to show the rendered image
 * which is currently stored in buffer.
 * After that, clear the buffer.
 * 
 * \returns Always 0
*/
extern int DrawFrame();

/**
 * Release all of resources which this header has allocated so far. 
 * 
 * \returns 0 if success, -1 if error.
*/
extern int TerminateMimicGL();

// vao_vbo.cc
/**
 * Changes the currently binded vao with given vao.
 * 
 * \param vao: The VAO to alternate.
*/
extern void bindVertexArray(struct VAO* vao);

/**
 * Changes the currently binded VBO with given VBO.
 * 
 * \param vbo: The VBO to alternate.
*/
extern void bindBuffer(struct VBO* vbo);

/**
 * Copys the data into buffer(VBO). The old content of buffer is lost.
 * 
 * \param size: The size of buffer. (Namely, The number of total data.)
 * \param data: The pointer which refers the starting address of
 *              data to be copied into. The data must be float type.
*/
extern void copyIntoBufferData(int size, float* data);

/**
 * Sets entry of currently binded VAO.
 * 
 * \param index: The index of entry of VAO to be set. For example,
 *               if the user wants to set the meta-data about the first
 *               attribute, it needs to be given as 1, and wants to set the data
 *               of 2nd attribute, it needs to be given as 2.. and so on.
 * \param size: The size of this attribute (per vertex).
 * \param stride: The stride this attribute appears.
 * \param offset: The total size of other attributes comming before
 *                this attribute (of first vertex).
*/
extern void setVaoPointer(int index, int size, int stride, int offset);

/**
 * Allocate new VAO.
 * 
 * \returns the allocated VAO's address.
*/
extern struct VAO* generateVertexArray();

/**
 * Allocate new VBO.
 * 
 * \returns the allocated VBO's address.
*/
extern struct VBO* generateBuffer();

/**
 * Free given vbo and its buffer.
*/
extern void deleteBuffer(struct VBO* vbo);

/**
 * Set vertex shader and fragment shader to be used for
 * vertex processing and fragment processing.
 * 
 * \param out_size: The size of out variables produced from vertex_shader.
 *                  Namely, the size of variables to be inputted to fragment
 *                  shader. All of out variables should be float,
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
    void (*vertex_shader)(float*[], mmath::Vec4<float>*, float[]),
    color_t (*fragment_shader)(struct Fragment*)
);


/**
 * Set the radius of point to be drawn
 * when DrawArray is called with type parameter as DRAW_POINTS.
 * 
 * \param radius: positive integer that represents the radius of point.
*/
extern void set_point_radius(int radius);

/**
 * Set the mode which determines
 * if the window shows the color of rendered image,
 * or the color of the depth of pixel (red: near - black: far)
 * vertex processing and fragment processing.
 * 
 * \param t: if t is given true, the window shows depth of pixel,
 *           but if not, the window shows just color of rendered image.
 */
extern void set_z_mode(bool t);

/**
 * Set the default background color to show
 * when any object is not placed at that pixel.
 * 
 * \param color: 32-bit color value to be set as background color.
*/
extern void set_clear_color(color_t color);
