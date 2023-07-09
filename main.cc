#include <iostream>
#include "mimic/include/mimicMath.hh"
#include "mimic/include/mimic.hh"
#include "src/include/SDL2/SDL.h"
#include "mimic/include/mimicMath.hh"

const int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 600;

mmath::Mat4x4<float> M;
mmath::Mat4x4<float> MVP;
void vertex_shader(float* attr[], mmath::Vec4<float>* pos, float out[])
{
	// Get attributes of vertex from VBO.
	auto position = reinterpret_cast<mmath::Vec3<float>*>(attr[0]);
	auto color = reinterpret_cast<mmath::Vec3<float>*>(attr[1]);

	mmath::Vec4<float> p(*position, 1);
	*pos = MVP * p;

	out[0] = attr[1][0];
	out[1] = attr[1][1];
	out[2] = attr[1][2];
}

int main(int argc, char* argv[])
{
    StartMimicGL(WINDOW_WIDTH, WINDOW_HEIGHT);

	float triangle[] =
	{
		0, 0, 0, 1, 0, 0,
		0.3, 0, 0.3, 0, 1, 0,
		0.3, 0, -.3, 0, 0, 1, 
		-10, 0, 0, 1, 0, 0,
		10, 0, 0, 0, 0, 1,
		0, 0, -10, 0, 1, 0,
		0, 0, 10, 0, 1, 0,
		0, 0, 0, 0, 0, 1, 
		0, 1, 0, 0, 0, 1,
		
		-0.5 ,  0.5 ,  0.5 ,  1, 1, 1, // v0
         0.5 , -0.5 ,  0.5 ,  1, 1, 1, // v2
         0.5 ,  0.5 ,  0.5 ,  1, 1, 1, // v1
                    
        -0.5 ,  0.5 ,  0.5 ,  1, 1, 1, // v0
        -0.5 , -0.5 ,  0.5 ,  1, 1, 1, // v3
         0.5 , -0.5 ,  0.5 ,  1, 1, 1, // v2
                    
        -0.5 ,  0.5 , -0.5 ,  1, 1, 1, // v4
         0.5 ,  0.5 , -0.5 ,  1, 1, 1, // v5
         0.5 , -0.5 , -0.5 ,  1, 1, 1, // v6
                    
        -0.5 ,  0.5 , -0.5 ,  1, 1, 1, // v4
         0.5 , -0.5 , -0.5 ,  1, 1, 1, // v6
        -0.5 , -0.5 , -0.5 ,  1, 1, 1, // v7
                    
        -0.5 ,  0.5 ,  0.5 ,  1, 1, 1, // v0
         0.5 ,  0.5 ,  0.5 ,  1, 1, 1, // v1
         0.5 ,  0.5 , -0.5 ,  1, 1, 1, // v5
                    
        -0.5 ,  0.5 ,  0.5 ,  1, 1, 1, // v0
         0.5 ,  0.5 , -0.5 ,  1, 1, 1, // v5
        -0.5 ,  0.5 , -0.5 ,  1, 1, 1, // v4
 
        -0.5 , -0.5 ,  0.5 ,  1, 1, 1, // v3
         0.5 , -0.5 , -0.5 ,  1, 1, 1, // v6
         0.5 , -0.5 ,  0.5 ,  1, 1, 1, // v2
                    
        -0.5 , -0.5 ,  0.5 ,  1, 1, 1, // v3
        -0.5 , -0.5 , -0.5 ,  1, 1, 1, // v7
         0.5 , -0.5 , -0.5 ,  1, 1, 1, // v6
                    
         0.5 ,  0.5 ,  0.5 ,  1, 1, 1, // v1
         0.5 , -0.5 ,  0.5 ,  1, 1, 1, // v2
         0.5 , -0.5 , -0.5 ,  1, 1, 1, // v6
                    
         0.5 ,  0.5 ,  0.5 ,  1, 1, 1, // v1
         0.5 , -0.5 , -0.5 ,  1, 1, 1, // v6
         0.5 ,  0.5 , -0.5 ,  1, 1, 1, // v5
                    
        -0.5 ,  0.5 ,  0.5 ,  1, 1, 1, // v0
        -0.5 , -0.5 , -0.5 ,  1, 1, 1, // v7
        -0.5 , -0.5 ,  0.5 ,  1, 1, 1, // v3
                    
        -0.5 ,  0.5 ,  0.5 ,  1, 1, 1, // v0
        -0.5 ,  0.5 , -0.5 ,  1, 1, 1, // v4
        -0.5 , -0.5 , -0.5 ,  1, 1, 1, // v7		
	};

	struct VAO* vao = generateVertexArray();
	bindVertexArray(vao);

	struct VBO* vbo = generateBuffer();
	bindBuffer(vbo);

	copyIntoBufferData(300, triangle);
	setVaoPointer(0, 3, 6, 0);
	setVaoPointer(1, 3, 6, 3);

	set_vertex_shader(vertex_shader);


    SDL_Event event;
    bool running = true;
	bool mouse_down = false;
	int pmx, pmy;
	int mouse_x, mouse_y;

	float camera_h = 5;
	mmath::Vec3<float> b_pos = {0, 0, 0};
    while(running)
    {
        while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;

			case SDL_KEYDOWN:
				switch(event.key.keysym.scancode)
				{
				case SDL_SCANCODE_S:
					running = false; break;
				case SDL_SCANCODE_LEFT:
					b_pos[0] -= 0.5; break;
				case SDL_SCANCODE_RIGHT:
					b_pos[0] += 0.5; break;
				case SDL_SCANCODE_UP:
					camera_h += 0.5; break;
				case SDL_SCANCODE_DOWN:
					camera_h -= 0.5; break;
				}
				break;

			case SDL_KEYUP:
				if (event.key.keysym.scancode == SDL_SCANCODE_S) {
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					mouse_down = true;
					SDL_GetRelativeMouseState(&pmx, &pmy);
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					mouse_down = false;
				}
				break;
			}
		}

		double t = SDL_GetTicks() / 500.0;
		M = mmath::translate(mmath::Vec3<float>{0, 0, 0});
		mmath::Mat4x4<float> V = mmath::lookat({5 * (float)cos(t / 10),
			camera_h, 5 * (float)sin(t / 10)}, {0, 0, 0}, {0, 1, 0});
		//mmath::Mat4x4<float> V = mmath::lookat({0, 5, 0}, {0, 0, 0}, {(float)cos(t / 10), 0, (float)sin(t / 10)});
		mmath::Mat4x4<float> P = mmath::perspective(45, 1, 1, 10);
		//mmath::Mat4x4<float> P = mmath::ortho(-3, 3, -3, 3, -3, 3); 

		MVP = P * V * M;

		// std::cout << M << std::endl;
		// std::cout << V << std::endl;
		// std::cout << P << std::endl;
		// std::cout << MVP << std::endl;

		M = mmath::translate(mmath::Vec3<float>(SDL_GetTicks() / 100, 100, 0))
			* mmath::scale(mmath::Vec3<float>(1 + SDL_GetTicks() / 10000.0, 1 + SDL_GetTicks() / 10000.0, 1));

		M = mmath::translate(mmath::Vec3<float>(400, 100, 0)) *
		mmath::rotate(mmath::Vec3<float>(0, 0, 1), SDL_GetTicks() / 300.0);
		
		//DrawArrays(0, 3, DRAW_LINES);
		DrawArrays(3, 6, DRAW_LINES);
		
		M = mmath::translate(mmath::Vec3<float>{b_pos[0], 3 * (float)cos(t), 0});
		MVP = P * V * M;

		DrawArrays(9, 36, DRAW_TRIANGLES_FRAME);
		DrawFrame();
    }

    TerminateMimicGL();
    return 0;
}