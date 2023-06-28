#include <iostream>
#include "mimic/include/mimicMath.hh"
#include "mimic/include/mimic.hh"
#include "src/include/SDL2/SDL.h"
#include "mimic/include/mimicMath.hh"

const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

mmath::Mat4x4<float> M;
void vertex_shader(float* attr[], mmath::Vec3<int>* pos, float out[])
{
	auto position = reinterpret_cast<mmath::Vec3<float>*>(attr[0]);
	auto color = reinterpret_cast<mmath::Vec3<float>*>(attr[1]);

	mmath::Vec4<float> p(*position, 1);
	p = M * p;

	*pos = mmath::Vec3<float>(p);

	out[0] = attr[1][0];
	out[1] = attr[1][1];
	out[2] = attr[1][2];
}

int main(int argc, char* argv[])
{
    StartMimicGL(WINDOW_WIDTH, WINDOW_HEIGHT);

	float triangle[] =
	{
		30, 30, 0, 1, 0, 0,
		30, 200, 0, 0, 1, 0,
		200, 30, 0, 0, 0, 1, 
		30, 60, 0, 1, 0, 0,
		30, 240, 0, 0, 1, 0,
		200, 560, 0, 0, 0, 1, 
	};

	struct VAO* vao = generateVertexArray();
	bindVertexArray(vao);

	struct VBO* vbo = generateBuffer();
	bindBuffer(vbo);

	copyIntoBufferData(36, triangle);
	setVaoPointer(0, 3, 6, 0);
	setVaoPointer(1, 3, 6, 3);

	M = mmath::translate(mmath::Vec3<float>(SDL_GetTicks() / 10, 100, 0));

	set_vertex_shader(vertex_shader);


    SDL_Event event;
    bool running = true;
    while(running)
    {
        while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_S) {
					running = false;
				}
				break;

			case SDL_KEYUP:
				if (event.key.keysym.scancode == SDL_SCANCODE_S) {
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
				}
				break;
			}
		}
		M = mmath::translate(mmath::Vec3<float>(SDL_GetTicks() / 100, 100, 0))
			* mmath::scale(mmath::Vec3<float>(SDL_GetTicks() / 100, SDL_GetTicks() / 100, 1));
		
		DrawArrays(0, 6, DRAW_LINE_STRIP);
		DrawFrame();
    }

    TerminateMimicGL();
    return 0;
}