
#include <SDL.h>

#pragma comment(lib, "sdl2.lib")
#pragma comment(lib, "sdl2main.lib")

extern int game_startup();
extern void game_update();
extern void game_shutdown();

SDL_Window* hWnd = nullptr;

int main(int argc, char* argv[]) {

	int result = 0;
	result = SDL_Init(SDL_INIT_VIDEO);

	hWnd = SDL_CreateWindow("My win32 gameloop", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		640, 
		480, 
		SDL_WINDOW_SHOWN);
	
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
