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

    context.renderer = SDL_CreateRenderer(context.window, -1, 0);

    context.texture = SDL_CreateTexture(context.renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
        window_w, window_h);

    context.window_w = window_w;
    context.window_h = window_h;

    return 0;    
}

std::vector<Fragment> fr;

void DrawArrays(int start, int number, int type)
{
    auto& vao_table = context.binded_vao->vao_table;

    switch(type)
    {
    case DRAW_POINTS:

    case DRAW_LINES: // Line    
        // Vertex Processing & Primitive Processing
        for(int i = start; i + 1 < start + number; i += 2)
        {
            PLine line;
            line.p1 = call_vertex_shader(i);
            line.p2 = call_vertex_shader(i + 1);
            
            // Scan Conversion & Fragment Processing
            draw_line_with_dda(line, 3, &fr);
        }
        break;
        
    case DRAW_LINE_STRIP: // Line    
        // Vertex Processing & Primitive Processing
        for(int i = start; i < start + number - 1; i++)
        {
            PLine line;
            line.p1 = call_vertex_shader(i);
            line.p2 = call_vertex_shader(i + 1);
            
            // Scan Conversion & Fragment Processing
            draw_line_with_dda(line, 3, &fr);
        }
        break;
    
    case DRAW_LINE_LOOP:
        for(int i = start; i <= start + number - 1; i++)
        {
            PLine line;
            line.p1 = call_vertex_shader(i);
            if(i == start + number - 1) line.p2 = call_vertex_shader(start);
            else line.p2 = call_vertex_shader(i + 1);
            
            // Scan Conversion & Fragment Processing
            draw_line_with_dda(line, 3, &fr);
        }
        break;
        
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