#include <iostream>
#include "mimic/include/mimicMath.hh"
#include "mimic/include/mimic.hh"
#include "src/include/SDL2/SDL.h"
#include "mimic/include/mimicMath.hh"
#include "mimic/include/fragment.hh"

const int WINDOW_WIDTH = 600, WINDOW_HEIGHT = 600;

mmath::Mat4x4<double> M;
mmath::Mat4x4<double> MVP;
mmath::Vec3<double> view_pos = {5, 0, 5};

void vertex_shader(double* attr[], mmath::Vec4<double>* pos, double out[])
{
	// Get attributes of vertex from VBO.
	auto position = reinterpret_cast<mmath::Vec3<double>*>(attr[0]);
	auto color = reinterpret_cast<mmath::Vec3<double>*>(attr[1]);

	mmath::Vec4<double> p(*position, 1);
	*pos = MVP * p;
	
	out[0] = attr[1][0];
	out[1] = attr[1][1];
	out[2] = attr[1][2];
}

void phong_vshader(double* attr[], mmath::Vec4<double>* pos, double out[])
{
	// Get attributes of vertex from VBO.
	auto position = *reinterpret_cast<mmath::Vec3<double>*>(attr[0]);
	auto normal = *reinterpret_cast<mmath::Vec3<double>*>(attr[1]);

	mmath::Vec4<double> p(position, 1);
	*pos = MVP * p;
	
	mmath::Mat3x3<double> m3(M);

	auto vec3_out = reinterpret_cast<mmath::Vec3<double>*>(out);
	vec3_out[0] = mmath::Vec3<double>(M * p);
	vec3_out[1] = m3.transpose().inverse() * normal;
}

color_t fragment_shader(struct Fragment* fragment)
{
	int r = (int)std::min(fragment->f_data[0] * 255, 255.0);
    int g = (int)std::min(fragment->f_data[1] * 255, 255.0);
    int b = (int)std::min(fragment->f_data[2] * 255, 255.0);
	
    return (r << 24) | (g << 16) | (b << 8);
}

color_t phong_fshader(struct Fragment* fragment)
{
	mmath::Vec3<double> light_pos(3, 2, 4);
	mmath::Vec3<double> light_color(1, 1, 1);
	mmath::Vec3<double> material_color(1.0, 0.0, 0.0);

	auto surface_pos = *reinterpret_cast<mmath::Vec3<double>*>(fragment->f_data);
	auto normal = *reinterpret_cast<mmath::Vec3<double>*>(
		fragment->f_data + 3);
	normal = normal.normalize();

	double shininess = 75.0;
	
	auto light_dir = (light_pos - surface_pos).normalize();

	// ambient
	auto ambient = mmath::mult_element_wise(light_color, material_color) * .1;
	
	// diffuse
	double diff = std::max(mmath::dot(normal, light_dir), .0);
	auto diffuse = mmath::mult_element_wise(light_color, material_color) * diff;

	auto view_dir = (view_pos - surface_pos).normalize();
	auto refl_dir = mmath::reflect(-1.0 * light_dir, normal);
	double spec = std::max(mmath::dot(view_dir, refl_dir), .0);
	auto specular = pow(spec, shininess) * mmath::mult_element_wise(material_color, {1, 1, 1});

	auto color = ambient + diffuse + specular;

	int r = (int)std::min(color.x * 255, 255.0);
    int g = (int)std::min(color.y * 255, 255.0);
    int b = (int)std::min(color.z * 255, 255.0);
	
    return (r << 24) | (g << 16) | (b << 8);
}

int main(int argc, char* argv[])
{
    StartMimicGL(WINDOW_WIDTH, WINDOW_HEIGHT);

	double box[] =
	{
		-1,  1,  1,  0,  0,  1, // v0
		 1, -1,  1,  0,  0,  1, // v2
		 1,  1,  1,  0,  0,  1, // v1

		-1,  1,  1,  0,  0,  1, // v0
		-1, -1,  1,  0,  0,  1, // v3
		 1, -1,  1,  0,  0,  1, // v2

		-1,  1, -1,  0,  0, -1, // v4
		 1,  1, -1,  0,  0, -1, // v5
		 1, -1, -1,  0,  0, -1, // v6

		-1,  1, -1,  0,  0, -1, // v4
		 1, -1, -1,  0,  0, -1, // v6
		-1, -1, -1,  0,  0, -1, // v7

		-1,  1,  1,  0,  1,  0, // v0
		 1,  1,  1,  0,  1,  0, // v1
		 1,  1, -1,  0,  1,  0, // v5

		-1,  1,  1,  0,  1,  0, // v0
		 1,  1, -1,  0,  1,  0, // v5
		-1,  1, -1,  0,  1,  0, // v4

		-1, -1,  1,  0, -1,  0, // v3
		 1, -1, -1,  0, -1,  0, // v6
		 1, -1,  1,  0, -1,  0, // v2

		-1, -1,  1,  0, -1,  0, // v3
		-1, -1, -1,  0, -1,  0, // v7
		 1, -1, -1,  0, -1,  0, // v6

		 1,  1,  1,  1,  0,  0, // v1
		 1, -1,  1,  1,  0,  0, // v2
		 1, -1, -1,  1,  0,  0, // v6

		 1,  1,  1,  1,  0,  0, // v1
		 1, -1, -1,  1,  0,  0, // v6
		 1,  1, -1,  1,  0,  0, // v5

		-1,  1,  1, -1,  0,  0, // v0
		-1, -1, -1, -1,  0,  0, // v7
		-1, -1,  1, -1,  0,  0, // v3

		-1,  1,  1, -1,  0,  0, // v0
		-1,  1, -1, -1,  0,  0, // v4
		-1, -1, -1, -1,  0,  0, // v7
	};

	double triangle[] =
	{
		0, 0, 0, 1, 0, 0,
		10, 0, 0, 0, 0, 1,
		0, 0, -10, 0, 1, 0,
		0, 0, 10, 0, 1, 0,
		0, 0, 0, 0, 0, 1, 
		0, 2, 0, 0, 0, 1,
		
		 0.0,  0.7,  0.7,  0.4, 1.0, 1.0, // v0
         0.7,  0.0,  0.7,  0.4, 1.0, 1.0, // v2
         0.7,  0.7,  0.7,  0.4, 1.0, 1.0, // v1
                    
         0.0,  0.7,  0.7,  0.4, 1.0, 1.0, // v0
         0.0,  0.0,  0.7,  0.4, 1.0, 1.0, // v3
         0.7,  0.0,  0.7,  0.4, 1.0, 1.0, // v2
                    
         0.0,  0.7,  0.0,  1.0, 0.4, 1.0, // v4
         0.7,  0.7,  0.0,  1.0, 0.4, 1.0, // v5
         0.7,  0.0,  0.0,  1.0, 0.4, 1.0, // v6
                    
         0.0,  0.7,  0.0,  1.0, 0.4, 1.0, // v4
         0.7,  0.0,  0.0,  1.0, 0.4, 1.0, // v6
         0.0,  0.0,  0.0,  1.0, 0.4, 1.0, // v7
                    
         0.0,  0.7,  0.7,  1.0, 1.0, 0.4, // v0
         0.7,  0.7,  0.7,  1.0, 1.0, 0.4, // v1
         0.7,  0.7,  0.0,  1.0, 1.0, 0.4, // v5
                    
         0.0,  0.7,  0.7,  1.0, 1.0, 0.4, // v0
         0.7,  0.7,  0.0,  1.0, 1.0, 0.4, // v5
         0.0,  0.7,  0.0,  1.0, 1.0, 0.4, // v4
 
         0.0,  0.0,  0.7,  0.4, 0.4, 1.0, // v3
         0.7,  0.0,  0.0,  0.4, 0.4, 1.0, // v6
         0.7,  0.0,  0.7,  0.4, 0.4, 1.0, // v2
                    
         0.0,  0.0,  0.7,  0.4, 0.4, 1.0, // v3
         0.0,  0.0,  0.0,  0.4, 0.4, 1.0, // v7
         0.7,  0.0,  0.0,  0.4, 0.4, 1.0, // v6
                    
         0.7,  0.7,  0.7,  1.0, 0.4, 0.4, // v1
         0.7,  0.0,  0.7,  1.0, 0.4, 0.4, // v2
         0.7,  0.0,  0.0,  1.0, 0.4, 0.4, // v6
                    
         0.7,  0.7,  0.7,  1.0, 0.4, 0.4, // v1
         0.7,  0.0,  0.0,  1.0, 0.4, 0.4, // v6
         0.7,  0.7,  0.0,  1.0, 0.4, 0.4, // v5
                    
         0.0,  0.7,  0.7,  1.0, 1.0, 1.0, // v0
         0.0,  0.0,  0.0,  1.0, 1.0, 1.0, // v7
         0.0,  0.0,  0.7,  1.0, 1.0, 1.0, // v3
                    
         0.0,  0.7,  0.7,  1.0, 1.0, 1.0, // v0
         0.0,  0.7,  0.0,  1.0, 1.0, 1.0, // v4
         0.0,  0.0,  0.0,  1.0, 1.0, 1.0, // v7  	

                  0.0,  0.7,  0.7,  1.0, 0.0, 0.0, // v0
         0.7,  0.0,  0.7,  0.4, 1.0, 1.0, // v2
         0.7,  0.7,  0.7,  0.4, 1.0, 1.0, // v1
                    
         0.0,  0.7,  0.7,  1.0, 0.0, 0.0, // v0
         0.0,  0.0,  0.7,  0.4, 1.0, 1.0, // v3
         0.7,  0.0,  0.7,  0.4, 1.0, 1.0, // v2
                    
         0.0,  0.7,  0.0,  1.0, 0.4, 1.0, // v4
         0.7,  0.7,  0.0,  1.0, 0.4, 1.0, // v5
         0.7,  0.0,  0.0,  1.0, 0.4, 1.0, // v6
                    
         0.0,  0.7,  0.0,  1.0, 0.4, 1.0, // v4
         0.7,  0.0,  0.0,  1.0, 0.4, 1.0, // v6
         0.0,  0.0,  0.0,  1.0, 0.4, 1.0, // v7
                    
         0.0,  0.7,  0.7,  1.0, 0.0, 0.0, // v0
         0.7,  0.7,  0.7,  1.0, 1.0, 0.4, // v1
         0.7,  0.7,  0.0,  1.0, 1.0, 0.4, // v5
                    
         0.0,  0.7,  0.7,  1.0, 0.0, 0.0, // v0
         0.7,  0.7,  0.0,  1.0, 1.0, 0.4, // v5
         0.0,  0.7,  0.0,  1.0, 1.0, 0.4, // v4
 
         0.0,  0.0,  0.7,  0.4, 0.4, 1.0, // v3
         0.7,  0.0,  0.0,  0.4, 0.4, 1.0, // v6
         0.7,  0.0,  0.7,  0.4, 0.4, 1.0, // v2
                    
         0.0,  0.0,  0.7,  0.4, 0.4, 1.0, // v3
         0.0,  0.0,  0.0,  0.4, 0.4, 1.0, // v7
         0.7,  0.0,  0.0,  0.4, 0.4, 1.0, // v6
                    
         0.7,  0.7,  0.7,  1.0, 0.4, 0.4, // v1
         0.7,  0.0,  0.7,  1.0, 0.4, 0.4, // v2
         0.7,  0.0,  0.0,  1.0, 0.4, 0.4, // v6
                    
         0.7,  0.7,  0.7,  1.0, 0.4, 0.4, // v1
         0.7,  0.0,  0.0,  1.0, 0.4, 0.4, // v6
         0.7,  0.7,  0.0,  1.0, 0.4, 0.4, // v5
                    
         0.0,  0.7,  0.7,  1.0, 0.0, 0.0, // v0
         0.0,  0.0,  0.0,  1.0, 1.0, 1.0, // v7
         0.0,  0.0,  0.7,  1.0, 1.0, 1.0, // v3
                    
         0.0,  0.7,  0.7,  1.0, 0.0, 0.0, // v0
         0.0,  0.7,  0.0,  1.0, 1.0, 1.0, // v4
         0.0,  0.0,  0.0,  1.0, 1.0, 1.0, // v7  	

         -1.0,  0.7,  0.7,  1.0, 0.0, 0.0, // v0
         -1.0,  0.7,  0.0,  0.0, 0.0, .0, // v4
         -1.0,  0.0,  0.0,  0.0, 0.0, .0, // v7	
	};

	struct VAO* vao = generateVertexArray();
	bindVertexArray(vao);

	struct VBO* vbo = generateBuffer();
	bindBuffer(vbo);
	copyIntoBufferData(sizeof(triangle) / sizeof(double), triangle);
	
	setVaoPointer(0, 3, 6, 0);
	setVaoPointer(1, 3, 6, 3);

	struct VAO* box_vao = generateVertexArray();
	bindVertexArray(box_vao);
		
	struct VBO* box_vbo = generateBuffer();
	bindBuffer(box_vbo);
	copyIntoBufferData(sizeof(box) / sizeof(double), box);

	setVaoPointer(0, 3, 6, 0);
	setVaoPointer(1, 3, 6, 3);

    SDL_Event event;
    bool running = true;
	bool mouse_down = false;
	int pmx, pmy;
	double rad = 5;
	int mouse_x, mouse_y;

	double camera_h = 0;
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
				case SDL_SCANCODE_D:
					set_z_mode(true); break;
				case SDL_SCANCODE_S:
					set_z_mode(false); break;
				case SDL_SCANCODE_ESCAPE:
					running = false; break;
				case SDL_SCANCODE_LEFT:
					rad -= 0.25; break;
				case SDL_SCANCODE_RIGHT:
					 rad += 0.25; break; 
				case SDL_SCANCODE_UP:
					camera_h += 0.1; break;
				case SDL_SCANCODE_DOWN:
					camera_h -= 0.1; break;
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

		double t = SDL_GetTicks() / 200.0;
		view_pos = {rad * (double)cos(-t / 10), camera_h, rad * (double)sin(-t / 10)};

		
		M = mmath::translate(mmath::Vec3<double>{0, 0, 0});
		mmath::Mat4x4<double> V = mmath::lookat(view_pos, {0, 0, 0}, {0, 1, 0});
		mmath::Mat4x4<double> P = mmath::perspective(45, 1, 1, 40);
		//mmath::Mat4x4<double> P = mmath::ortho(-3, 3, -3, 3, -3, 3); 

		MVP = P * V * M;

		// std::cout << M << std::endl;
		// std::cout << V << std::endl;
		// std::cout << P << std::endl;
		// std::cout << MVP << std::endl;
		
		bindVertexArray(vao);
		bindBuffer(vbo);
		set_shaders(6, vertex_shader, fragment_shader);
		DrawArrays(0, 6, DRAW_LINES);
		
		M = mmath::translate(mmath::Vec3<double>(0)) * mmath::scale(mmath::Vec3<double>(1, 1, 1));
		MVP = P * V * M;

		bindVertexArray(box_vao);
		bindBuffer(box_vbo);
		set_shaders(6, phong_vshader, phong_fshader);

		DrawArrays(0, 36, DRAW_TRIANGLES);
		DrawFrame();
    }

    TerminateMimicGL();
    return 0;
}