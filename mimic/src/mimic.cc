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

std::vector<Fragment> fr;

void DrawArrays(const uint32_t start, const uint32_t number, DrawingType type)
{
    auto& vao_table = context.binded_vao->vao_table;

    switch(type)
    {
        case DRAW_LINES:
        // Vertex Processing & Primitive Processing
        for(int i = start; i + 1 < start + number; i += 2)
        {
            VertexShaderOutput p1 = call_vertex_shader(i);
            VertexShaderOutput p2 = call_vertex_shader(i + 1);

            // Clipping with respect to x, y, z coordinate.
            clipping_line(p1, p2, 3);

            // Scan Conversion & Fragment Processing
            draw_line_with_dda(p1, p2, 3, &fr);
        }
        break;
    

        case DRAW_LINE_LOOP:
        {
            // Draw line linking between first vertex and last vertex.
            VertexShaderOutput p1 = call_vertex_shader(start);
            VertexShaderOutput p2 = call_vertex_shader(start + number - 1);

            // Clipping with respect to x, y, z coordinate.
            if(clipping_line(p1, p2, 3))
            {
                // Scan Conversion & Fragment Processing
                draw_line_with_dda(p1, p2, 3, &fr);
            }
            
            // And work like DRAW_LINE_STRIP
        }

        case DRAW_LINE_STRIP:
        {
            // Ignore the given number is small.
            if(number <= 1) break;

            VertexShaderOutput odd_vertex, even_vertex;

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
                draw_line_with_dda(even_vertex, odd_vertex, 3, &fr);
            }
            break;
        }

        case DRAW_TRIANGLES_FRAME:
        {
            
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

int TerminateMimicGL()
{
    context.binded_vao = nullptr;
    for(auto &vao : context.vaos)
    {
        delete vao;
    }
    context.vaos.clear();
    
    context.binded_vbo = nullptr;
    for(auto &vbo : context.vbos)
    {
        delete[] vbo->object;
        delete vbo;
    }
    context.vbos.clear();

    SDL_DestroyTexture(context.texture);
    SDL_DestroyRenderer(context.renderer);
    SDL_DestroyWindow(context.window);

    SDL_Quit();
    return 0;
}