#include "SDL.h"

extern SDL_Surface *screen;

int min(int a, int b) {
	return (a < b) ? a : b;
}

int max(int a, int b) {
	return (a > b) ? a : b;
}

void draw_surface(SDL_Surface *surf, int x, int y) {
	SDL_Rect src, dest;

	src.x = src.y = 0;
	src.w = surf->w, src.h = surf->h;

	dest.x = x, dest.y = y;
	dest.w = surf->w, dest.h = surf->h;

	if (SDL_BlitSurface(surf, &src, screen, &dest)) {
		fprintf(stderr, "SDL_BlitSurface : %s\n", SDL_GetError());
		exit(1);
	}
}