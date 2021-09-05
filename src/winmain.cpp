
#include <SDL.h>

#pragma comment(lib, "sdl2.lib")
#pragma comment(lib, "sdl2main.lib")

extern int game_startup();
extern void game_update();
extern void game_shutdown();

SDL_Window* hWnd = nullptr;
int width = 640;
int height = 480;
bool windowed = true;

void toggle_window();

int main(int argc, char* argv[]) {

	int result = 0;
	result = SDL_Init(SDL_INIT_VIDEO);

    Uint32 flags = SDL_WINDOW_SHOWN;
    if (!windowed)
        flags |= SDL_WINDOW_FULLSCREEN;

	hWnd = SDL_CreateWindow("My win32 gameloop - ESC to exit, F1 for fullscreen/window", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		width, 
		height, 
		flags);
	
	SDL_SetWindowInputFocus(hWnd);

	game_startup();

    bool quit = false;
    SDL_Event e;

    while (!quit)
    {

        // handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_F1:
                    windowed = !windowed;
                    toggle_window();
                    break;
                default:
                    break;
                }
            }
        }

        int w;
        int h;
        SDL_GetWindowSize(hWnd, &w, &h);

        SDL_Surface* surface = SDL_GetWindowSurface(hWnd);
        Uint32 skyblue = SDL_MapRGB(surface->format, 100, 149, 237);
        SDL_FillRect(surface, NULL, skyblue);
        SDL_UpdateWindowSurface(hWnd);

        game_update();

    }

    game_shutdown();

    SDL_DestroyWindow(hWnd);
    hWnd = nullptr;

    SDL_Quit();

    return 0;
}

void toggle_window()
{
    int flags = SDL_GetWindowFlags(hWnd);
    int fullscreen = (flags & SDL_WINDOW_FULLSCREEN) ? true : false;

    SDL_SetWindowFullscreen(hWnd, !fullscreen);
    SDL_SetWindowInputFocus(hWnd);
}
