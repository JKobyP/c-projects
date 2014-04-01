#ifndef SDLGRID_H
#define SDLGRID_H
struct tile{
	SDL_Rect rectangle;
	int live;
	int dying;
	int born;
};

struct tile** map_init(int rows, int cols, int size);
void map_destroy(struct tile** map, int rows);

#endif
