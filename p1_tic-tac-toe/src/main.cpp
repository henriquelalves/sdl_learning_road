#include <SDL2/SDL.h>
#include <stdio.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Globals
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

// Board
int board[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
enum tiles
{
	TILE_EMPTY = 0,
	TILE_X,
	TILE_Y
};

// Game states
enum states
{
	STATE_BEGIN,
	STATE_PLAY_1,
	STATE_PLAY_2,
	END
};
int current_state = 0;

int main(int argc, char *args[])
{
	// Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);
	gWindow = SDL_CreateWindow("Tic-Tac-Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Loop
	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		//Update screen
		SDL_RenderPresent(gRenderer);
	}
}