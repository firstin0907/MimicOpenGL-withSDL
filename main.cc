#include <iostream>
#include <SDL2/SDL.h>
#include "mimic/include/mimic.hh"

const int WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600;

int main(int argc, char* argv[])
{
    StartMimicGL(WINDOW_WIDTH, WINDOW_HEIGHT);

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
    }

    TerminateMimicGL();
    return 0;
}