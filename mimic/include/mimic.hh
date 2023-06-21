struct Context
{
    struct SDL_Window *window;
};

extern int StartMimicGL(int window_w, int window_h);

extern int DrawFrame();

extern int TerminateMimicGL();