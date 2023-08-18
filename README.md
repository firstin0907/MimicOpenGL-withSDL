# MimicOpenGL-withSDL
The small toy 3D software rasterizer using SDL2 library.



## Getting start
### Build 🏗
You can build this project with Makefile. To build it, just type
```
make all
```
in command.
### Write main.cc ✒
By modifying main.cc source file and rebuilding project, you can change what is rendered. The user(programmer) is intended to write the code with including only "mimic/include/mimic.hh" header file which provides enough operations for rendering. For more details about the operations this project provides, see [mimic/include/mimic.hh](/mimic/include/mimic.hh) and its comments.


## Brief Structure of Rendering Pipeline
Rendering Pipeline, except for per-sample operation, is mimicked by calling `DrawArray(const uint32_t start, const uint32_t number, DrawingType type)` function in [mimic/include/mimic.hh](/mimic/include/mimic.hh), with data of `context` instance which is set through other function calls before calling this function.

Behavior of this function varies slightly with the `type` parameter, but overall behavior is almost similar.

### Vertex Processing
In this stage, the function calculates the position of vertex, by calling vertex shader, per each vertex. Vertex shader is descripted by user and stored in `context` instance previously by user calling `void set_shaders(const uint32_t out_size, void (*vertex_shader)(float*[], mmath::Vec4<float>*, float[]), color_t (*fragment_shader)(struct Fragment*))` function.

### Vertex Post-Processing
In this stage, some operations are conducted in order to fit vertex shader
output for the pixel of output window. Perspective division(i.e. dividing xyz-coordinate by w-coordinate) is conducted in this stage.

In case of lines, the function clips the line preemptively and adjusts the coordinates and attributes.

And in case of triangles, back-face culling is conducted.

## Scan Conversion(Rasterization)
## Fragment Processing
## Per-sample Operations


## Example
### Video(Link) 🎞
Implementation of phong shading, through using this project.

[![Phong Shading](https://img.youtube.com/vi/F5bhYDi9R2M/0.jpg)](https://youtu.be/F5bhYDi9R2M) 
