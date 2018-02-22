#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>

// Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

// Game methods
void draw_menu();
void draw_game();
void draw_end();
void check_win();
int check_row(int n);
int check_col(int n);

void reset_game();
void select_tile(int col, int row);
int get_tile(int col, int row);
void ai_move();
void draw_circle(int cx, int cy, int r);

// Globals
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

// Board
int board[] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
enum tiles
{
	TILE_EMPTY = -1,
	TILE_X,
	TILE_Y
};

// Game states
enum states
{
	STATE_BEGIN,
	STATE_PLAY_1,
	STATE_PLAY_2,
	STATE_END
};
int current_state = 0;
int current_player = 0;
int win = -2;

// Font
TTF_Font *gFontSans = NULL;

void draw_circle(int cx, int cy, int r)
{
	int r2 = r * r;
	int x = 0, x2 = 0, dx2 = 1;
	int y = r, y2 = r2, dy2 = 2 * y - 1;
	int sum = r2;

	while (x <= y)
	{
		SDL_RenderDrawPoint(gRenderer, cx + x, cy + y);
		SDL_RenderDrawPoint(gRenderer, cx + x, cy - y);
		SDL_RenderDrawPoint(gRenderer, cx - x, cy + y);
		SDL_RenderDrawPoint(gRenderer, cx - x, cy - y);
		SDL_RenderDrawPoint(gRenderer, cx + y, cy + x);
		SDL_RenderDrawPoint(gRenderer, cx + y, cy - x);
		SDL_RenderDrawPoint(gRenderer, cx - y, cy + x);
		SDL_RenderDrawPoint(gRenderer, cx - y, cy - x);

		sum -= dx2;
		x2 += dx2;
		x++;
		dx2 += 2;
		if (sum <= y2)
		{
			y--;
			y2 -= dy2;
			dy2 -= 2;
		}
	}
}

void draw_text(int x, int y, int width, int height, char *str)
{
	SDL_Color Black = {0x00, 0x00, 0x00};

	SDL_Surface *surfaceMessage = TTF_RenderText_Solid(gFontSans, str, Black);
	SDL_Texture *Message = SDL_CreateTextureFromSurface(gRenderer, surfaceMessage);

	SDL_Rect Message_rect;   //create a rect
	Message_rect.x = x;		 //controls the rect's x coordinate
	Message_rect.y = y;		 // controls the rect's y coordinte
	Message_rect.w = width;  // controls the width of the rect
	Message_rect.h = height; // controls the height of the rect

	SDL_RenderCopy(gRenderer, Message, NULL, &Message_rect);

	// Destroying surface and texture
	SDL_DestroyTexture(Message);
	SDL_FreeSurface(surfaceMessage);
}

void draw_menu()
{
	draw_text(50, 10, 500, 200, "Ugly Tic-Tac-Toe");
	draw_text(10, 300, 400, 100, "One player: press 1");
	draw_text(10, 500, 400, 100, "Two players: press 2");
}

void draw_game()
{
	// Draw player turn
	if (current_player == 0)
		draw_text(250, 10, 100, 50, "Player 1");
	else
		draw_text(250, 10, 100, 50, "Player 2");

	// Draw board
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawLine(gRenderer, 200, 0, 200, 600);
	SDL_RenderDrawLine(gRenderer, 400, 0, 400, 600);
	SDL_RenderDrawLine(gRenderer, 0, 200, 600, 200);
	SDL_RenderDrawLine(gRenderer, 0, 400, 600, 400);

	// Player 1
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	for (int i = 0; i < 9; i++)
	{
		if (board[i] == 0)
		{
			int cx = (i % 3) * 200 + 100;
			int cy = (i / 3) * 200 + 100;
			SDL_RenderDrawLine(gRenderer, cx - 90, cy - 90, cx + 90, cy + 90);
			SDL_RenderDrawLine(gRenderer, cx + 90, cy - 90, cx - 90, cy + 90);
		}
	}

	// Player 2
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
	for (int i = 0; i < 9; i++)
	{
		if (board[i] == 1)
		{
			int cx = (i % 3) * 200 + 100;
			int cy = (i / 3) * 200 + 100;
			draw_circle(cx, cy, 95);
		}
	}
}

void draw_end()
{
	if (win == -1)
	{
		draw_text(50, 200, 500, 200, "It's a draw!");
	}
	else if (win == 0)
	{
		draw_text(50, 200, 500, 200, "Player 1 won!");
	}
	else if (win == 1)
	{
		draw_text(50, 200, 500, 200, "Player 2 won!");
	}
}

void check_win()
{
	int n = 0;
	if (check_row(n) > -1 || check_row(++n) > -1 || check_row(++n) > -1)
	{
		//printf("oi\n");
		win = get_tile(0, n);
		return;
	}
	n = 0;
	if (check_col(n) > -1 || check_row(++n) > -1 || check_row(++n) > -1)
	{
		win = get_tile(n, 0);
		return;
	}
	if (get_tile(0, 0) == get_tile(1, 1) && get_tile(1, 1) == get_tile(2, 2))
	{
		if (get_tile(0, 0) != TILE_EMPTY)
		{
			win = get_tile(0, 0);
			return;
		}
	}
	if (get_tile(2, 0) == get_tile(1, 1) && get_tile(1, 1) == get_tile(0, 2))
	{
		if (get_tile(2, 0) != TILE_EMPTY)
		{
			win = get_tile(2, 0);
			return;
		}
	}

	for (int i = 0; i < 9; i++)
	{
		if (board[i] == TILE_EMPTY)
		{
			win = -2;
			return;
		}
	}

	win = -1;
}

int get_tile(int row, int col)
{
	return board[(row * 3) + col];
}

int check_row(int n)
{
	if (get_tile(0, n) == get_tile(1, n) && get_tile(1, n) == get_tile(2, n))
	{
		if (get_tile(0, n) != TILE_EMPTY)
			return get_tile(0, n);
	}
	return -1;
}

int check_col(int n)
{
	if (get_tile(n, 0) == get_tile(n, 1) && get_tile(n, 1) == get_tile(n, 2))
	{
		if (get_tile(n, 0) != TILE_EMPTY)
			return get_tile(n, 0);
	}
	return -1;
}

void reset_game()
{
	for (int i = 0; i < 9; i++)
	{
		board[i] = -1;
	}
	win = -2;
	current_player = 0;
}

void select_tile(int col, int row)
{
	int n = (row * 3) + col;

	if (board[n] == -1)
	{
		board[n] = current_player;
		current_player = (current_player + 1) % 2;
	}
}

void ai_move()
{
	int r_tile = 0;
	do
	{
		r_tile = std::rand() % 8;
	} while (board[r_tile] != -1);

	board[r_tile] = 1;
	current_player = 0;

	check_win();
}

int main(int argc, char *args[])
{
	// Random
	std::srand(std::time(nullptr));

	// Initialize SDL
	SDL_Init(SDL_INIT_VIDEO);
	gWindow = SDL_CreateWindow("Tic-Tac-Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_SOFTWARE);
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

	// Initialize fonts system
	TTF_Init();
	gFontSans = TTF_OpenFont("./FreeSans.ttf", 24); //this opens a font style and sets a size

	if (!gFontSans)
	{
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}

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

			if (current_state == STATE_BEGIN)
			{
				if (e.type == SDL_KEYDOWN)
				{
					if (e.key.repeat == 0 && e.key.keysym.scancode == SDL_SCANCODE_1)
					{
						reset_game();
						current_state = STATE_PLAY_1;
					}
					if (e.key.repeat == 0 && e.key.keysym.scancode == SDL_SCANCODE_2)
					{
						reset_game();
						current_state = STATE_PLAY_2;
					}
				}
			}

			else if (current_state == STATE_PLAY_1 && current_player == 0)
			{
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					int col = e.button.x / 200;
					int row = e.button.y / 200;

					select_tile(col, row);
					check_win();
				}
			}

			else if (current_state == STATE_PLAY_2)
			{
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					printf("foi\n");
					int col = e.button.x / 200;
					int row = e.button.y / 200;

					select_tile(col, row);
					check_win();
				}
			}

			else if (current_state == STATE_END)
			{
				if (e.type == SDL_MOUSEBUTTONDOWN)
				{
					current_state = STATE_BEGIN;
				}
			}
		}

		// Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		// Drawing states
		if (current_state == STATE_BEGIN)
		{
			draw_menu();
		}
		else if (current_state == STATE_PLAY_1 || current_state == STATE_PLAY_2 || current_state == STATE_END)
		{
			draw_game();
		}
		if (current_state == STATE_END)
		{
			draw_end();
		}
		// Update screen
		SDL_RenderPresent(gRenderer);

		// Check win
		if (current_state == STATE_PLAY_1 || current_state == STATE_PLAY_2)
		{
			if (win != -2)
			{
				current_state = STATE_END;
			}
			else
			{

				// Its AI turn?
				if (current_state == STATE_PLAY_1 && current_player == 1)
				{
					ai_move();
				}
			}
		}
	}
}