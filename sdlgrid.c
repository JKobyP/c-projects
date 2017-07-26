#include <SDL.h>

struct tile{
	SDL_Rect rectangle;
	int live;
	int dying;
	int born;
};

struct tile** map_init(int rows, int cols, int size)
{
	struct tile** map = malloc(sizeof(struct tile)*rows);
	int i;
	int j;
	//initialize the game board (map)
	for (i = 0; i < rows; i++)
	{
		map[i] = malloc(sizeof(struct tile)*cols);
		for(j = 0; j < cols; j++)
		{
			map[i][j].rectangle.x = j*size;
			map[i][j].rectangle.y = i*size;
			map[i][j].rectangle.w = size;
			map[i][j].rectangle.h = size;
			map[i][j].live = 0;
			map[i][j].dying = 0;
			map[i][j].born = 0;
		}
	}
	return map;
}

void map_destroy(struct tile** map, int rows)
{
	int i;
	//deallocate memory for game board
	for(i = 0; i < rows; i++)
		free(map[i]);
	free(map);
}
