//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <assert.h>
//Screen dimension constants
const int SCREEN_WIDTH = 12*70;
const int SCREEN_HEIGHT = 12*70;
const int TILE_SIZE = 12;
int ROWS = 0;
int COLS = 0;
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;

#include "../sdlgrid.h"
#include "conwaygol.h"

int initialize_sdl()
{
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return 0;
	}
	else
	{
		//Create window
		window = SDL_CreateWindow( "Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, 			SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			return 0;
		}
		else
			return 1;
	}
}


int main( int argc, char* args[] )
{
	ROWS = SCREEN_HEIGHT/TILE_SIZE;
	COLS = SCREEN_WIDTH/TILE_SIZE;
	//Allocate memory for our game board
	struct tile** map = map_init(ROWS, COLS, TILE_SIZE);

	//initialize sdl
	assert(initialize_sdl());

	//Get widow surface
	screenSurface = SDL_GetWindowSurface(window);

	SDL_Event e;
	int quit = 1;

	//x and y are mouse positions
	int x = -1;
	int y = -1;
	int pause = 1;
	int delay = 150;	

	//main game loop
	while(quit == 1)
	{
		map_update(map, screenSurface);
		eventsLoop(e, map, screenSurface, &x, &y, &quit, &pause, &delay);


		//Update the surface
		SDL_UpdateWindowSurface( window );
		SDL_Delay(delay);
		if(pause == 1)
		{
			while(pause == 1 && quit == 1)
			{
				eventsLoop(e, map, screenSurface, &x, &y, &quit, &pause, &delay);
				SDL_UpdateWindowSurface( window );
			}
		}
	}
	map_destroy(map, ROWS);
	close_sdl();
	return 0;
}

void close_sdl()
{
	//Destroy window
	SDL_DestroyWindow( window );
	//Quit SDL subsystems
	SDL_Quit();
}

//First, iterate through game board and evaluate if tiles should be killed or created
//Then, change life status and draw rectangle
void map_update(struct tile** map, SDL_Surface* screenSurface)
{
	int i; int j;
	for(i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLS; j++)
		{
			struct tile *neighbors = tile_getNeighbors(map, i, j);
			int living = tile_liveNeighbors(neighbors);
			free(neighbors);
			if(living > 3)
				map[i][j].dying = 1;
			else if(living == 3)
				map[i][j].born = 1;
			else if(living <= 1)
				map[i][j].dying = 1;
		}
	}

	for(i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLS; j++)
		{
			if(map[i][j].dying == 1)
			{
				map[i][j].live = 0;
				map[i][j].dying = 0;
			}
			if(map[i][j].born == 1)
			{
				map[i][j].live = 1;
				map[i][j].born = 0;
			}
			if (map[i][j].live == 1)
			{
				SDL_FillRect( screenSurface, &(map[i][j].rectangle), SDL_MapRGB( screenSurface->format, 255, 255, 255 ) );
			}
			else
			{
				SDL_FillRect( screenSurface, &(map[i][j].rectangle), SDL_MapRGB( screenSurface->format, 0, 0, 0 ) );
			}
		}
	}
}

struct tile *tile_getNeighbors(struct tile** map, int i, int j)
{
	struct tile *neighbors = malloc(sizeof(struct tile[9]));
	int k;
	int p;
	int c = 0;
	for (k = i-1; k <= i+1; k++)
	{
		for(p = j-1; p <= j+1; p++)
		{

			if (k == i && p == j)
				continue;
			int x = k; int y = p;
			if(k < 0)
				x = COLS-1;
			else if (k>COLS-1)
				x = 0;
			if (p < 0)
				y = ROWS-1;
			else if (p>ROWS-1)
				y = 1;
			neighbors[c] = map[x][y];
			c++;
		}
	}
	return neighbors;

}

int tile_liveNeighbors(struct tile *neighbors)
{
	int i;
	int c = 0;
	for (i = 0; i < 8; i++)
	{
		if (neighbors[i].live !=0)
			c++;
	}
	return c;
}

void eventsLoop(SDL_Event e, struct tile** map, SDL_Surface* screenSurface, int* x, int* y, int* quit, int* pause, int* delay)
{
	while(SDL_PollEvent( &e ) > 0)
	{
		//If the user has Xed out the window
		if( e.type == SDL_QUIT )
		{
			//Quit the program
			*quit = 0;
		}
		else if(e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN)
		{
			if(e.type == SDL_MOUSEBUTTONDOWN)
			{
				handleMouseEvent(e, map, screenSurface);
				*x = e.button.x/TILE_SIZE;
				*y = e.button.y/TILE_SIZE;
			}
			else if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
			{
				if(e.button.x/TILE_SIZE != *x || e.button.y/TILE_SIZE != *y)
				{
					handleMouseEvent(e, map, screenSurface);
					*x = e.button.x/TILE_SIZE;
					*y = e.button.y/TILE_SIZE;
				}
			}
		}
		else if(e.type == SDL_KEYDOWN)
		{
			handleKeyEvent(e, delay, pause);
		}
	}
} 
void handleKeyEvent(SDL_Event e, int* delay, int* pause)
{
	switch(e.key.keysym.sym)
	{
		case SDLK_SPACE:
			*pause = !*pause;
			break;
		//speed up or slow down
		case SDLK_UP:
			*delay -= 10;
			if(*delay <= 50)
				*delay = 50;
			break;
		case SDLK_DOWN:
			*delay += 10;
			break;
	}
}

void handleMouseEvent(SDL_Event e, struct tile** map, SDL_Surface* screenSurface)
{
	int x = e.button.x;
	int y = e.button.y;
	assert(0 <= y && y <= SCREEN_HEIGHT && 0 <= x && x <= SCREEN_WIDTH);

	struct tile* toChange = &map[y/TILE_SIZE][x/TILE_SIZE];
	if(toChange->live == 1)
	{
		toChange->live = 0;
		SDL_FillRect( screenSurface, &(toChange->rectangle), SDL_MapRGB( screenSurface->format, 0, 0, 0 ) );

	}
	else
	{
		toChange->live = 1;
		SDL_FillRect( screenSurface, &(toChange->rectangle), SDL_MapRGB( screenSurface->format, 255, 255, 255) );
	}
}

