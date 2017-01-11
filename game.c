/**
 * game.c -- main file of game pokemon ball
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SDL.h"
#include "alien.h"

// Surfaces 
SDL_Surface *screen = NULL;
SDL_Surface *aliens[2] = { NULL, NULL };


void init_alien_surfaces() {
	SDL_Surface *img;
	
	// ship
	img = SDL_LoadBMP("img/ship.bmp");
	SDL_SetColorKey(img, SDL_SRCCOLORKEY, SDL_MapRGB(img->format, 0, 0, 0));
	aliens[SHIP] = img;

	// pokemon
	img = SDL_LoadBMP("img/pokemon.bmp");
	SDL_SetColorKey(img, SDL_SRCCOLORKEY, SDL_MapRGB(img->format, 255, 255, 255));
	aliens[POKEMON] = img;
}

void init_all() {
	// Initialize SDL 
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Couldn't initialize SDL:%s\n", SDL_GetError());
		exit(1);	
	}
	atexit(SDL_Quit);

	// Initialize Video Mode
	SDL_WM_SetIcon(SDL_LoadBMP("img/icon.bmp"), NULL);
	screen = SDL_SetVideoMode(1024, 768, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (screen == NULL) {
		fprintf(stderr, "Couldn't set 1024x768 video mode: %s\n", SDL_GetError());
		exit(2);
	}
	SDL_WM_SetCaption("Pokemon Ball", NULL); 
	init_alien_surfaces();
}

int main() {
	srand(time(NULL));
	init_all();
	int done = 0;
	
	alien ship = create_alien(SHIP, 437, 600);
	draw_alien(&ship);
	SDL_Flip(screen);

	alien pokemon = create_alien(POKEMON, 337, 600);
	draw_alien(&pokemon);
	SDL_Flip(screen);

	fprintf(stderr, "%d\n", has_collided(&ship, &pokemon));

	while (!done) {
		SDL_Event event;
		SDL_WaitEvent(&event);
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT: 
					done = 1;
					break;
				default:
					break;
			}
		}
	}
	return 0;
}