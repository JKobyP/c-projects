#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../sdlgrid.h"

//Screen dimension constants
const int SCREEN_WIDTH = 12*70;
const int SCREEN_HEIGHT = 12*70;
const int TILE_SIZE = 12;
int ROWS = 0;
int COLS = 0;
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;

struct Node{
	struct int[2] *value;
	struct Node *next;
};

struct LinkedList{
	struct Node *head;
	struct Node *tail;
	struct Node *secondLast;
	int size;
};

void setNext(struct Node *node, struct Node *next)
{
	assert (node != NULL && next != NULL);
	
	node->next = next;
}

struct LinkedList *LinkedList_create()
{
	struct LinkedList *list = malloc(sizeof(struct LinkedList));
	assert (list != NULL);
	
	list->size = 0;
	list->head = NULL;
	return list;
}

struct Node *Node_create(int[2] value)
{
	struct Node *node = malloc(sizeof(struct Node));
	assert (node != NULL);

	node->value = value;
	node->next = NULL;

	return node;
}

void setNext(struct Node *node, struct Node *next)
{
	assert (node != NULL && next != NULL);
	
	node->next = next;
}
void removeLast(struct LinkedList *list)
{
	int i;
	struct Node tmp = list->head;
	for(i = 0; i < (list->size)-4; i++)
		tmp = tmp->next;
//necessary?	free(tail);
	list->tail = list->secondLast;
	list->secondLast = tmp;
}
void insert(struct LinkedList *list, int[2] *value, int i)
{
	struct Node *node = Node_create(value);
	assert(list != NULL);
	assert(node != NULL);

	if(list->size == 0)
	{
		list->head = node;
		list->size++;

		return;
	}
	if(i > list->size)
		i = list->size;
	if(i == 0)
	{
		setNext(node, list->head);
		list->head = node;
	}
	else
	{
		int j;
		struct Node *temp = list->head;
		for(j = 0; j < (i-1); j++)
		{
			temp = temp->next;
		}
		setNext(node, temp->next);
		setNext(temp, node);
	}
	if(i == size)
	{
		list->secondLast = list->tail;
		list->tail = node;
	}	
	if(i == size-1)
		list->secondLast = node;
	list->size++;
}

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

void close_sdl()
{
	//Destroy window
	SDL_DestroyWindow( window );
	//Quit SDL subsystems
	SDL_Quit();
}

void map_update(struct tile** map, SDL_Surface *screenSurface, struct snake *mysnake)
{
	
	int[2] headLoc = mysnake->list->head->*value;
	switch(snake->dir)
	{
	case NORTH:
		headLoc[0]--;
		break;
	case EAST:
		headLoc[1]++;
	case SOUTH:
		headLoc[0]++;
	case WEST:
		headLoc[1]--;
	}
	//if the snake isn't eating
	if(map[headLoc[0]][headLoc[1]]->live == 0)
	{
		removeLast(mysnake->last);
	}
	else
		makeFood(map, mysnake);
	insert(mysnake->list, {headLoc[0],headLoc[1]}, 0);

	//light up the snake! (and food)
	int i;
	int j;	
	struct Node tmp = mysnake->list->head;
	for(i = 0; i < size; i++)
	{
		map[tmp->value[0]][tmp->value[1]]->live = 1;
		tmp = tmp->next;
	}

	for(i = 0; i < ROWS; i++)
	{
		for(j = 0; j < COLS; j++)
		{
			struct tile *mytile = map[i][j]'
			if (mytile->live == 1)
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

void makeFood(struct tile** map, snake* mysnake)
{
	int[2] random = {rand()%ROWS,rand()%COLS};
	while(!contains(mysnake, random))
	{
		random = {rand()%ROWS,rand()%COLS};
	}
	map[random[0]][random[1]]->live = 1;
}

int contains(mysnake, random)
{
	struct Node tmp = mysnake->list->head;
	for(i = 0; i < size; i++)
	{
		if(tmp->value[0] == random[0] && tmp->value[1] == random[1])
			return 0;
		tmp = tmp->next;
	}
	return 1;
}

enum direction {NORTH, EAST, SOUTH, WEST};

struct snake {
	LinkedList *list;
	direction dir;
};

struct snake *snake_create()
{
	int x = ROWS/2; int y = COLS/2;
	struct snake* mysnake;
	mysnake->list = LinkedList_create();
	insert(mysnake->list, &map[x][y], 0);
	insert(mysnake->list, &map[x+1][y], 0);
	insert(mysnake->list, &map[x+2][y], 0);
	direction = NORTH;
	return mysnake; 
}
/*void snake_destroy()
{
	LinkedList_destroy();
}
*/
void snake_append(struct snake *mysnake)
{
	int[2] tailLoc = mysnake->list->tail->*value;
	int[2] secondLastLoc = mysnake->list->secondLast->*value;
	if(tailLoc[0] == secondLastLoc[0])
	{
		if(tailLoc[1] < secondLastLoc[1])
			insert(mysnake->list, {tailLoc[0],tailLoc[1]-1});
		else
			insert(mysnake->list, {tailLoc[0],tailLoc[1]+1});
	}
	else if (tailLoc[0] > secondLastLoc[0])
		insert(mysnake->list, {tailLoc[0]+1,tailLoc[1]});
	else
		insert(mysnake->list, {tailLoc[0]-1,tailLoc[1]});
}

int main(int argc char* args[])
{
	ROWS = SCREEN_HEIGHT/TILE_SIZE;
	COLS = SCREEN_WIDTH/TILE_SIZE;
	assert(initialize_sdl());
	struct tile** map = map_init(ROWS, COLS, TILE_SIZE);
	struct snake *mysnake = snake_create();

	
	//initialize sdl
	assert(initialize_sdl());

	//Get widow surface
	screenSurface = SDL_GetWindowSurface(window);

	SDL_Event e;
	int quit = 1;

	int pause = 1;
	int delay = 500;	

	//main game loop
	while(quit == 1)
	{
		map_update(map, screenSurface, mysnake);
//		eventsLoop(e, map, screenSurface, &quit, &pause, &delay);


		//Update the surface
		SDL_UpdateWindowSurface(window);
		SDL_Delay(delay);
		if(pause == 1)
		{
			while(pause == 1 && quit == 1)
			{
//				eventsLoop(e, map, screenSurface, &quit, &pause, &delay);
				SDL_UpdateWindowSurface(window);
			}
		}
	}
	map_destroy(map, ROWS);
	close_sdl();
	return 0;
}