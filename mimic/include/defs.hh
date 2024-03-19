#pragma once

#include <vector>

#include "fragment.hh"
#include "primitive.hh"
#include "mimicMath.hh"

#define MAX_VERTEX_ATTRIBS 8

typedef int color_t;

struct Context
{
    struct SDL_Window* window = nullptr;
    struct SDL_Texture* texture = nullptr;
    struct SDL_Renderer* renderer = nullptr;

    struct VAO* binded_vao = nullptr;
    struct VBO* binded_vbo = nullptr;

    std::vector<struct VAO*> vaos;
    std::vector<struct VBO*> vbos;

    void (*vertex_shader)(float*[], mmath::Vec4<float>*, float[]) = nullptr;
    color_t (*fragment_shader)(struct Fragment*) = nullptr;
    
    // number of out variables of vertex shader
    uint32_t vshader_out_data_size = 0;
    
    // address of memory to store out variables of vertex shader
    float* vshader_out_data_buf = nullptr;
    float* fshader_out_data_buf = nullptr;

    struct 
    {
        int     point_radius; 
        color_t clear_color;
        bool    z_mode;
    } drawing_options;

    float* z_buffer = nullptr;
    int* color_buffer = nullptr;

    int frame_cnt;
    time_t frame_time[16];

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
        float* pointer;
    } vao_table[MAX_VERTEX_ATTRIBS];
};

struct VBO
{
    float* object;
};

struct VshaderOutput
{
    mmath::Vec4<float> pos;
    float* data;
};

// vertex_processer.cc
extern VshaderOutput call_vertex_shader(int vertex_idx, float* var_buffer);

bool vertex_post_processing_for_lines(VshaderOutput &p1, VshaderOutput& p2);
bool vertex_post_processing_for_points(VshaderOutput &p);

// per_sample_operator.cc
extern int perSampleOperation(Context* context,
    ShadedFragment* fragment);

// scan_converser.cc
extern bool clipping_line(VshaderOutput& p1, VshaderOutput& p2);
extern void draw_point(const VshaderOutput& p);
extern void draw_line_with_dda(const VshaderOutput& p1,
    const VshaderOutput& p2);
extern void draw_triangle(const VshaderOutput& p1, const VshaderOutput& p2,
    const VshaderOutput& p3);

// fragment_processor.cc
extern ShadedFragment call_fragment_shader(struct Fragment* fragment);
