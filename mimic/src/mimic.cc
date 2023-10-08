#include <SDL2/SDL.h>
#include "../include/mimic.hh"
#include "../include/defs.hh"
#include <iostream>

Context context;

int StartMimicGL(const char* window_title, int window_w, int window_h)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    
    context.window = SDL_CreateWindow(
        window_title, SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, window_w, window_h, SDL_WINDOW_ALLOW_HIGHDPI
    );
    if(context.window == nullptr) return -1;

    context.renderer = SDL_CreateRenderer(context.window, -1,
        SDL_RENDERER_ACCELERATED);
    if(context.renderer == nullptr) return -1;

    context.texture = SDL_CreateTexture(context.renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
        window_w, window_h);
    if(context.texture == nullptr) return -1;

    context.window_w = window_w;
    context.window_h = window_h;

    context.color_buffer = new int[window_w * window_h];
    context.z_buffer = new float[window_w * window_h];

    
    std::fill(context.z_buffer, context.z_buffer + window_w * window_h, 100.0);
    std::fill(context.color_buffer, context.color_buffer + window_w * window_h,
        context.drawing_options.clear_color);

    context.drawing_options.point_radius = 1;
    context.drawing_options.clear_color = 0x333333FF; // light gray.

    return 0;    
}


void DrawArrays(const uint32_t start, const uint32_t number, DrawingType type)
{
    auto& vao_table = context.binded_vao->vao_table;

    auto* buffer = context.vshader_out_data_buf;
    uint32_t var_sz = context.vshader_out_data_size;

    auto duplicate_vshader_output = [&](const VshaderOutput& instance)
    {
        VshaderOutput new_instance = {instance.pos, instance.data + var_sz * 3};
        for(int i = 0; i < var_sz; i++)
            new_instance.data[i] = instance.data[i];
        
        return new_instance;
    };

    switch(type)
    {
        case DRAW_POINTS:
        for(int i = start; i < start + number; i++)
        {
            // Vertex Processing
            VshaderOutput p1 = call_vertex_shader(i, buffer);

            // Vertex Post-Processing
            vertex_post_processing_for_points(p1);

            // No need to clipping 
            // Scan Conversion & Fragment Processing
            draw_point(p1);
        }
        break;


        case DRAW_LINES:
        for(int i = start; i + 1 < start + number; i += 2)
        {
            // Vertex Processing
            VshaderOutput p1 = call_vertex_shader(i, buffer);
            VshaderOutput p2 = call_vertex_shader(i + 1, buffer + var_sz);
            
            // Vertex Post-Processing
            vertex_post_processing_for_lines(p1, p2);

            // Clipping with respect to x, y, z coordinate.
            if(!clipping_line(p1, p2)) continue;

            // Scan Conversion & Fragment Processing
            draw_line_with_dda(p1, p2);
        }
        break;
    

        case DRAW_LINE_LOOP:
        {
            // Draw line linking between first vertex and last vertex.
            
            // Vertex Processing
            VshaderOutput p1 = call_vertex_shader(start, buffer);
            VshaderOutput p2 = call_vertex_shader(start + number - 1, buffer + var_sz);

            // Vertex Post-Processing
            vertex_post_processing_for_lines(p1, p2);

            // Clipping with respect to x, y, z coordinate.
            if(clipping_line(p1, p2))
                // Scan Conversion & Fragment Processing
                draw_line_with_dda(p1, p2);
            
            // And work like DRAW_LINE_STRIP if there are more lines to be drawn
            if(number <= 2) break;
        }

        case DRAW_LINE_STRIP:
        {
            // Ignore the given number is small.
            if(number <= 1) break;

            VshaderOutput odd_vertex, even_vertex;
            VshaderOutput odd_vertex_dup, even_vertex_dup;

            if(start % 2 == 0) even_vertex_dup = call_vertex_shader(start, buffer);
            else odd_vertex_dup = call_vertex_shader(start, buffer + var_sz);

            // Vertex Processing & Primitive Processing
            for(int i = start + 1; i < start + number; i++)
            {
                // Reuse previously calculated output,
                // only calculate new one vertex.

                if(i % 2 == 0)
                {
                    even_vertex = call_vertex_shader(i, buffer);
                    even_vertex_dup =  duplicate_vshader_output(even_vertex);

                    vertex_post_processing_for_lines(odd_vertex_dup, even_vertex);

                    // Clipping with respect to x, y, z coordinate.
                    if(!clipping_line(odd_vertex_dup, even_vertex)) continue;
                    
                    // Scan Conversion & Fragment Processing
                    draw_line_with_dda(odd_vertex_dup, even_vertex);
                }
                else
                {
                    odd_vertex = call_vertex_shader(i, buffer + var_sz);
                    odd_vertex_dup = duplicate_vshader_output(odd_vertex);

                    vertex_post_processing_for_lines(odd_vertex, even_vertex_dup);

                    // Clipping with respect to x, y, z coordinate.
                    if(!clipping_line(odd_vertex, even_vertex_dup)) continue;
                    
                    // Scan Conversion & Fragment Processing
                    draw_line_with_dda(odd_vertex, even_vertex_dup);
                }
            }
            break;
        }

        case DRAW_TRIANGLES_FRAME:
        {
            for(int i = start + 2; i < start + number; i += 3)
            {
                // Vertex Processing
                VshaderOutput p1 = call_vertex_shader(i - 2, buffer);
                VshaderOutput p2 = call_vertex_shader(i - 1, buffer + var_sz);
                VshaderOutput p3 = call_vertex_shader(i, buffer + var_sz * 2);

                // Each of vertex is needed to compose line, twice.
                // Thus, duplicate once for each of them.
                auto p1_dup = duplicate_vshader_output(p1);
                auto p2_dup = duplicate_vshader_output(p2);
                auto p3_dup = duplicate_vshader_output(p3);

                // Vertex Post-Processing
                vertex_post_processing_for_lines(p1, p2_dup);
                vertex_post_processing_for_lines(p2, p3_dup);
                vertex_post_processing_for_lines(p3, p1_dup);

                // Clipping with respect to x, y, z coordinate.
                if(!clipping_line(p1, p2)) continue;

                // Scan Conversion & Fragment Processing
                draw_line_with_dda(p1, p2_dup);
                draw_line_with_dda(p2, p3_dup);
                draw_line_with_dda(p3, p1_dup);
            }
            break;
        }

        
        case DRAW_TRIANGLES:
        {
            for(int i = start + 2; i < start + number; i += 3)
            {
                // Vertex Processing
                VshaderOutput p1 = call_vertex_shader(i - 2, buffer);
                VshaderOutput p2 = call_vertex_shader(i - 1, buffer + var_sz);
                VshaderOutput p3 = call_vertex_shader(i, buffer + var_sz * 2);
                
                vertex_post_processing_for_points(p1);
                vertex_post_processing_for_points(p2);
                vertex_post_processing_for_points(p3);

                mmath::Vec3<float> p1_pos(p1.pos), p2_pos(p2.pos), p3_pos(p3.pos);

                // Back-face culling
                auto polygon_normal =
                    mmath::cross(p2_pos - p1_pos, p3_pos - p1_pos);
                if(mmath::dot(polygon_normal, {0, 0, 1}) >= 0) continue;

                // Scan Conversion & Fragment Processing
                draw_triangle(p1, p2, p3);
            }
            break;
        }
        
    }
}

int DrawFrame()
{
    int* texture_pixels;
    int pitch;
    const int n_pixel = context.window_w * context.window_h;

    if(SDL_LockTexture(context.texture, NULL, (void **)&texture_pixels, &pitch) < 0)
    {
        return -1;
    }
    std::copy(context.color_buffer, context.color_buffer + n_pixel,
        texture_pixels);

    SDL_UnlockTexture(context.texture);
    SDL_RenderCopy(context.renderer, context.texture, NULL, NULL);
    SDL_RenderPresent(context.renderer);

    std::fill(context.z_buffer, context.z_buffer + n_pixel, 100.0);
    std::fill(context.color_buffer, context.color_buffer + n_pixel,
        context.drawing_options.clear_color);

        

    return 0;
}

void set_shaders(const uint32_t out_size,
    void (*vertex_shader)(float*[], mmath::Vec4<float>*, float[]),
    color_t (*fragment_shader)(struct Fragment*))
{
    context.vertex_shader = vertex_shader;
    context.fragment_shader = fragment_shader;
    context.vshader_out_data_size = out_size;

    if(context.vshader_out_data_buf != nullptr)
        delete[] context.vshader_out_data_buf;
    if(context.fshader_out_data_buf != nullptr)
        delete[] context.fshader_out_data_buf;

    context.vshader_out_data_buf = new float[out_size * 6];
    context.fshader_out_data_buf = new float[out_size * 6];
}

int TerminateMimicGL()
{
    delete[] context.color_buffer;
    context.color_buffer = nullptr;

    delete[] context.z_buffer;
    context.z_buffer = nullptr;

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