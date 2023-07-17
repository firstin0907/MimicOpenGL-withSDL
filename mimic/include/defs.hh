#pragma once

#include "fragment.hh"
#include "primitive.hh"
#include "mimicMath.hh"
#include <vector>

#define MAX_VERTEX_ATTRIBS 4 

typedef int color_t;

struct Context
{
    struct SDL_Window* window;
    struct SDL_Texture* texture;
    struct SDL_Renderer* renderer;

    struct VAO* binded_vao;
    struct VBO* binded_vbo;

    std::vector<struct VAO*> vaos;
    std::vector<struct VBO*> vbos;

    void (*vertex_shader)(double*[], mmath::Vec4<double>*, double[]) = nullptr;
    color_t (*fragment_shader)(struct Fragment*) = nullptr;
    
    // number of out variables of vertex shader
    uint32_t vshader_out_data_size = 0;
    
    // address of memory to store out variables of vertex shader
    double* vshader_out_data_buf = nullptr;
    double* fshader_out_data_buf = nullptr;

    int window_w, window_h;
};

extern Context context;

struct VAO
{
    int size;
    struct
    {
        int index;
        int size;
        int stride;
        double* pointer;
    } vao_table[MAX_VERTEX_ATTRIBS];
};

struct VBO
{
    double* object;
};

struct VshaderOutput
{
    mmath::Vec4<double> pos;
    double* data;
    int tp;
};

// vertex_processer.cc
extern VshaderOutput call_vertex_shader(int vertex_number);

bool vertex_post_processing_for_lines(VshaderOutput &p1, VshaderOutput& p2);
bool vertex_post_processing_for_points(VshaderOutput &p);

// per_sample_operator.cc
extern int perSampleOperation(Context* context,
    std::vector<ShadedFragment>* fragments);

// scan_converser.cc
extern bool clipping_line(VshaderOutput& p1, VshaderOutput& p2,
    int data_size);
extern void draw_line_with_dda(const VshaderOutput& p1,
    const VshaderOutput& p2, std::vector<ShadedFragment>* fragments);
extern void draw_triangle(const VshaderOutput& p1, const VshaderOutput& p2,
    const VshaderOutput& p3, std::vector<ShadedFragment>* fragments);

// fragment_processor.cc
extern ShadedFragment call_fragment_shader(struct Fragment* fragment);
