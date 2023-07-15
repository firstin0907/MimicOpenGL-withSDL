#include <SDL2/SDL.h>
#include "../include/mimic.hh"
#include "../include/defs.hh"
#include <iostream>

Context context;

int StartMimicGL(int window_w, int window_h)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    context.window = SDL_CreateWindow(
        "Hello World", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, window_w, window_h, SDL_WINDOW_ALLOW_HIGHDPI
    );

    if(context.window == nullptr) return -1;

    context.renderer = SDL_CreateRenderer(context.window, -1, SDL_RENDERER_ACCELERATED);

    context.texture = SDL_CreateTexture(context.renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
        window_w, window_h);

    context.window_w = window_w;
    context.window_h = window_h;

    return 0;    
}

std::vector<ShadedFragment> fr;

void DrawArrays(const uint32_t start, const uint32_t number, DrawingType type)
{
    auto& vao_table = context.binded_vao->vao_table;
    switch(type)
    {
        case DRAW_LINES:
        // Vertex Processing & Primitive Processing
        for(int i = start; i + 1 < start + number; i += 2)
        {
            VshaderOutput p1 = call_vertex_shader(i);
            VshaderOutput p2 = call_vertex_shader(i + 1);

            // Clipping with respect to x, y, z coordinate.
            if(!clipping_line(p1, p2, 3)) continue;

            // Scan Conversion & Fragment Processing
            draw_line_with_dda(p1, p2, &fr);
        }
        break;
    

        case DRAW_LINE_LOOP:
        {
            // Draw line linking between first vertex and last vertex.
            VshaderOutput p1 = call_vertex_shader(start);
            VshaderOutput p2 = call_vertex_shader(start + number - 1);

            // Clipping with respect to x, y, z coordinate.
            if(clipping_line(p1, p2, 3))
            {
                // Scan Conversion & Fragment Processing
                draw_line_with_dda(p1, p2, &fr);
            }
            
            // And work like DRAW_LINE_STRIP
        }

        case DRAW_LINE_STRIP:
        {
            // Ignore the given number is small.
            if(number <= 1) break;

            VshaderOutput odd_vertex, even_vertex;

            if(start % 2 == 0) even_vertex = call_vertex_shader(start);
            else odd_vertex = call_vertex_shader(start);

            // Vertex Processing & Primitive Processing
            for(int i = start + 1; i < start + number; i++)
            {
                // Reuse previously calculated output,
                // only calculate new one vertex.

                if(i % 2 == 0) even_vertex = call_vertex_shader(i);
                else odd_vertex = call_vertex_shader(i);
                
                // Clipping with respect to x, y, z coordinate.
                if(!clipping_line(even_vertex, odd_vertex, 3)) continue;

                // Scan Conversion & Fragment Processing
                draw_line_with_dda(even_vertex, odd_vertex, &fr);
            }
            break;
        }

        case DRAW_TRIANGLES_FRAME:
        {
            
        }

        
        case DRAW_TRIANGLES:
        {
            for(int i = start + 2; i < start + number; i += 3)
            {
                // Vertex Processing & Primitive Processing
                VshaderOutput p1 = call_vertex_shader(i - 2);
                VshaderOutput p2 = call_vertex_shader(i - 1);
                VshaderOutput p3 = call_vertex_shader(i);

                mmath::Vec3<double> p1_pos(p1.pos), p2_pos(p2.pos), p3_pos(p3.pos);

                // Back-face culling
                auto polygon_normal =
                    mmath::cross(p2_pos - p1_pos, p3_pos - p1_pos);
                if(mmath::dot(polygon_normal, {0, 0, 1}) >= 0) continue;

                // Scan Conversion & Fragment Processing
                draw_triangle(p1, p2, p3, &fr);
            }
            break;
        }
        
    }
}

int DrawFrame()
{
    // Per-sample Opeartions
    perSampleOperation(&context, &fr);
    fr.clear();
    
    SDL_RenderCopy(context.renderer, context.texture, NULL, NULL);

    SDL_RenderPresent(context.renderer);

    return 0;
}

void set_shaders(const uint32_t out_size,
    void (*vertex_shader)(double*[], mmath::Vec4<double>*, double[]),
    color_t (*fragment_shader)(struct Fragment*))
{
    context.vertex_shader = vertex_shader;
    context.fragment_shader = fragment_shader;
    context.vshader_out_data_size = out_size;

    
    if(context.vshader_out_data_buf != nullptr)
        delete[] context.vshader_out_data_buf;
    if(context.fshader_out_data_buf != nullptr)
        delete[] context.fshader_out_data_buf;

    context.vshader_out_data_buf = new double[out_size * 4];
    context.fshader_out_data_buf = new double[out_size * 4];
}

int TerminateMimicGL()
{
    // clear all vaos.
    context.binded_vao = nullptr;
    for(auto &vao : context.vaos)  delete vao;
    context.vaos.clear();
    
    // clear all vbos.
    context.binded_vbo = nullptr;
    for(auto &vbo : context.vbos) deleteBuffer(vbo);
    context.vbos.clear();

    delete[] context.vshader_out_data_buf;
    context.vshader_out_data_buf = nullptr;

    delete[] context.fshader_out_data_buf;
    context.fshader_out_data_buf = nullptr;

    SDL_DestroyTexture(context.texture);
    SDL_DestroyRenderer(context.renderer);
    SDL_DestroyWindow(context.window);

    SDL_Quit();
    return 0;
}