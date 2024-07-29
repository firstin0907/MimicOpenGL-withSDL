# MimicOpenGL-withSDL
The small toy 3D software rasterizer using SDL2 library.
This project seems to be built well only on Windows yet.

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
The whole rendering pipeline, from vertex processing stage to per-sample operation stage, is mimicked by calling `DrawArray(const uint32_t start, const uint32_t number, DrawingType type)` function in [mimic/include/mimic.hh](/mimic/include/mimic.hh), with data of `context` instance(global) which is set through other function calls before calling this function.

Behavior of this function varies slightly with the `type` parameter, but overall behavior is almost similar.

### Vertex Processing
In this stage, the function calculates the position of vertex, by calling vertex shader, per each vertex. Vertex shader is descripted by user and stored in `context` instance previously by user calling `void set_shaders(const uint32_t out_size, void (*vertex_shader)(float*[], mmath::Vec4<float>*, float[]), color_t (*fragment_shader)(struct Fragment*))` function.

### Vertex Post-Processing
In this stage, some operations are conducted in order to fit vertex shader
output for the pixel of output window. Perspective division(i.e. dividing xyz-coordinate by w-coordinate) is conducted in this stage.

In case of lines, the function clips the line preemptively and adjusts the coordinates and attributes.

And in case of triangles, back-face culling is conducted.

### Scan Conversion(Rasterization)
The primitives, composed of vertices, are converted into fragments(pixel) in this stage. Namely, the rasterizer determines which pixel is contained in each shape, such as triangle or line. For this, it conducts DDA algorithm for lines, and conducts naive algorithm(checking for all pixel if it is in primitive) for triangles.

I plan to improve it more efficiently soon.

### Fragment Processing
The fragments, created in Scan Conversion stage, are shaded in this stage, in the way which fragment shader describes.
Like Vertex Processing stage, fragment shader is descripted by user and stored in `context` instance previously by user calling `void set_shaders(const uint32_t out_size, void (*vertex_shader)(float*[], mmath::Vec4<float>*, float[]), color_t (*fragment_shader)(struct Fragment*))` function.

### Per-sample Operations
Lastly, the shaded fragments are filtered by z-buffering, so that only the frontmost fragment are displayed at rendering window in this stage.

## Example
### Video(Link) 🎞
Implementation of phong shading, through using this project.

[![Phong Shading](https://img.youtube.com/vi/F5bhYDi9R2M/0.jpg)](https://youtu.be/F5bhYDi9R2M) 
