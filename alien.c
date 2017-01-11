#include "SDL.h"
#include "alien.h"

extern SDL_Surface *screen;
extern SDL_Surface *aliens[2];

alien create_alien(int type, int x, int y) {
	alien a;
	a.type = type;
	a.x = x;
	a.y = y;
	a.x_vel = a.y_vel = 0;
	a.x_acc = a.y_acc = 0;
	a.r = (aliens[type]->h + aliens[type]->w) / 4;
	return a;
}

void draw_alien(alien *a) {
	SDL_Rect src, dest;

	src.x = src.y = 0;
	src.w = aliens[a->type]->w, src.h = aliens[a->type]->h;

	dest.x = a->x, dest.y = a->y;
	dest.w = aliens[a->type]->w, dest.h = aliens[a->type]->h;

	SDL_BlitSurface(aliens[a->type], &src, screen, &dest);
}