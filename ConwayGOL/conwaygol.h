#ifndef EXGRAPHICS_H
#define EXGRAPHICS_H

struct tile;
void map_update(struct tile** map, SDL_Surface* screenSurface);
struct tile *tile_getNeighbors(struct tile** map, int i, int j);
int tile_liveNeighbors(struct tile *neighbors);
void handleMouseEvent(SDL_Event e, struct tile** map, SDL_Surface* screenSurface);
void eventsLoop(SDL_Event e, struct tile**, SDL_Surface* screenSurface, int* x, int* y, int* quit, int* pause);
int main( int argc, char* args[] );

#endif
