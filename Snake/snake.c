#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../sdlgrid.h"

//Screen dimension constants
const int SCREEN_WIDTH = 12*70;
const int SCREEN_HEIGHT = 12*50;
const int TILE_SIZE = 12;
int ROWS = 0;
int COLS = 0;
int foodx =  2;
int foody = 2;
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;


typedef enum {NORTH, EAST, SOUTH, WEST} direction;

struct Node{
	int value[2];
	struct Node *next;
};

void Node_destroy(struct Node *node);

struct LinkedList{
	struct Node *head;
	struct Node *tail;
	struct Node *secondLast;
	int size;
};


struct snake {
	struct LinkedList *list;
	direction dir;
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

struct Node *Node_create(int value[2])
{
	struct Node *node = malloc(sizeof(struct Node));
	assert (node != NULL);

	node->value[0] = value[0];
	node->value[1] = value[1];
	node->next = NULL;

	return node;
}


void removeLast(struct snake *mysnake, struct tile** map)
{
	int i;
	struct Node *tmp = mysnake->list->head;
	for(i = 0; i < (mysnake->list->size)-3; i++)
		tmp = tmp->next;
	map[mysnake->list->tail->value[0]][mysnake->list->tail->value[1]].live = 0;
	struct Node *toDestroy = mysnake->list->tail;
	mysnake->list->tail = mysnake->list->secondLast;
	mysnake->list->tail->next = NULL;
	Node_destroy(toDestroy);
	mysnake->list->secondLast = tmp;
	mysnake->list->size--;
}

void insert(struct LinkedList *list, int value[2], int i)
{
	struct Node *node = Node_create(value);
	assert(list != NULL);
	assert(node != NULL);

	if(list->size == 0)
	{
		list->head = node;
		list->tail = node;
		list->secondLast = node;
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
	if(i == list->size)
	{
		list->secondLast = list->tail;
		list->tail = node;
	}	
	if(i == list->size-1)
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
		window = SDL_CreateWindow( "Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, 			SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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

int contains(struct snake *mysnake, int coord[2])
{ 
	int i;
	struct Node *tmp = mysnake->list->head;
	for(i = 0; i < mysnake->list->size; i++)
	{
//		printf("\nSnake body coordinate: (%d,%d) \t Checked Against: (%d,%d)",tmp->value[0],tmp->value[1],coord[0],coord[1]);
		if(tmp->value[0] == coord[0] && tmp->value[1] == coord[1])
			return 1;
		tmp = tmp->next;
	}
	return 0;
}

void makeFood(struct tile** map, struct snake* mysnake)
{
	
	int random[2] = {rand()%(ROWS-1),rand()%(COLS-1)};
	while(contains(mysnake, random) == 1)
	{
		random[0]= rand()%(ROWS-1);
		random[1] = rand()%(COLS-1);
	}
//	printf("\n\tThe coordinate for food is: (%d,%d)", random[0],random[1]);
	map[random[0]][random[1]].live = 1;
}

void Node_destroy(struct Node *node)
{
	assert(node != NULL);

	if(node->next != NULL)
	{
		struct Node *nodeToDestroy = node->next;
		Node_destroy(nodeToDestroy);
	}	
	free(node);
}

void LinkedList_destroy(struct LinkedList *list)
{
	assert(list != NULL);
	struct Node *head = list->head;
	
	if(head!=NULL)
		Node_destroy(head);
	free(list);
}

void snake_destroy(struct snake *mysnake)
{
	LinkedList_destroy(mysnake->list);
	free(mysnake);	
}

void loser(struct snake *mysnake, struct tile** map)
{
	printf("\n***You lose! Thanks for playing!***\n");
	map_destroy(map, ROWS);	
	snake_destroy(mysnake);
	close_sdl();
}

void map_update(struct tile** map, SDL_Surface *screenSurface, struct snake *mysnake, int *quit, int *delay)
{
	
	int headLoc[2];
	headLoc[0] = mysnake->list->head->value[0];
	headLoc[1] = mysnake->list->head->value[1];
	switch(mysnake->dir)
	{
		case NORTH:
			headLoc[0]--;
			break;
		case EAST:
			headLoc[1]++;
			break;
		case SOUTH:
			headLoc[0]++;
			break;
		case WEST:
			headLoc[1]--;
			break;
	}
	//losing condition - hits walls	
	if(headLoc[0] >= ROWS || headLoc[0] < 0 || headLoc[1] >= COLS || headLoc[1] < 0)
	{
		*quit = 0;
		return;
	}
	//if the snake isn't eating
	if(map[headLoc[0]][headLoc[1]].live == 0)
	{
		removeLast(mysnake, map);
	}
	else
	{
		if(contains(mysnake, headLoc) == 1)
		{
			*quit = 0;
			return;
		}
		makeFood(map, mysnake);
		if(*delay > 5)		
			*delay -= 5;
	}
	
	insert(mysnake->list, headLoc, 0);
	map[headLoc[0]][headLoc[1]].live = 1;
	//light up the snake! (and food)
	int i;
	int j;	
//	struct Node *tmp = mysnake->list->head;
//	for(i = 0; i < mysnake->list->size && tmp != NULL; i++)
//	{
//		map[tmp->value[0]][tmp->value[1]].live = 1;			
//		tmp = tmp->next;
//	}

	for(i = 0; i < ROWS; i++)
	{
		for(j = 0; j < COLS; j++)
		{
			struct tile *mytile = &map[i][j];
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

struct snake *snake_create(struct tile** map)
{
	int x = ROWS/2; int y = COLS/2;
	struct snake* mysnake = malloc(sizeof(struct snake));
	mysnake->list = LinkedList_create();
	insert(mysnake->list, (int[2]){x,y}, 0);
	insert(mysnake->list, (int[2]){x,y+1}, 0);
	insert(mysnake->list, (int[2]){x,y+2}, 0);
	mysnake->dir = NORTH;
	map[x][y].live = 1;
	map[x][y+1].live=1;
	map[x][y+2].live=1;
	makeFood(map, mysnake);
	return mysnake; 
}


/*void snake_append(struct snake *mysnake)
{
	int tailLoc[2];
	tailLoc[0] = mysnake->list->tail->value[0];
	tailLoc[0] = mysnake->list->tail->value[1];
	int secondLastLoc[2] = {mysnake->list->secondLast->value[0],mysnake->list->secondLast->value[1]};
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
*/

void handleKeyEvent(SDL_Event e, struct snake *mysnake, int* pause)
{
	switch(e.key.keysym.sym)
	{
		case SDLK_SPACE:
			*pause = !*pause;
			break;
		case SDLK_UP:
			if(mysnake->dir != SOUTH)
				mysnake->dir = NORTH;
			break;
		case SDLK_DOWN:
			if(mysnake->dir != NORTH)
				mysnake->dir = SOUTH;
			break;
		case SDLK_LEFT:
			if(mysnake->dir != EAST)
				mysnake->dir = WEST;
			break;
		case SDLK_RIGHT:
			if(mysnake->dir != WEST)
				mysnake->dir = EAST;
			break;
	}
}

void eventsLoop(SDL_Event e, struct tile** map, struct snake *mysnake, SDL_Surface* screenSurface, int* quit, int* pause, int* delay)
{
	while(SDL_PollEvent( &e ) > 0)
	{
		//If the user has Xed out the window
		if(e.type == SDL_QUIT )
		{
			//Quit the program
			*quit = 0;
		}
		else if(e.type == SDL_KEYDOWN)
		{
			if(e.key.keysym.sym == SDLK_ESCAPE)
				*quit = 0;
			handleKeyEvent(e, mysnake, pause);
		}
	}
} 



int main(int argc, char* args[])
{
	srand(0);
	ROWS = SCREEN_HEIGHT/TILE_SIZE;
	COLS = SCREEN_WIDTH/TILE_SIZE;
	assert(initialize_sdl());
	struct tile** map = map_init(ROWS, COLS, TILE_SIZE);
	struct snake *mysnake = snake_create(map);

	//Get widow surface
	screenSurface = SDL_GetWindowSurface(window);

	SDL_Event e;
	int quit = 1;

	int pause = 1;
	int delay = 200;	

	//main game loop
	while(quit == 1)
	{
		map_update(map, screenSurface, mysnake, &quit, &delay);
		eventsLoop(e, map, mysnake, screenSurface, &quit, &pause, &delay);


		//Update the surface
		SDL_UpdateWindowSurface(window);
		SDL_Delay(delay);
		if(pause == 1)
		{
			while(pause == 1 && quit == 1)
			{
				eventsLoop(e, map, mysnake, screenSurface, &quit, &pause, &delay);
			}
		}
	}
	loser(mysnake, map);
}
