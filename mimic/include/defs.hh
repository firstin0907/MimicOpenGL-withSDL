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

    void (*vertex_shader)(float*[], mmath::Vec4<float>*, float[]);

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

// vertex_processer.cc
extern PPoint call_vertex_shader(int vertex_number);

// per_sample_operator.cc
extern int perSampleOperation(Context* context, std::vector<Fragment>* fragments);

// scan_converser.cc
extern void draw_line_with_dda(PLine line, int data_length,
    std::vector<Fragment>* fragments);

// fragment_processor.cc
extern void call_fragment_shader(struct Fragment* fragment);
