/**
 * game.c -- main file of game pokemon ball
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SDL.h"

// Surfaces 
SDL_Surface *screen = NULL;

void initAll() {
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
}

int main() {
	srand(time(NULL));
	initAll();
	int done = 0;
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
	SDL_Flip(screen);
	return 0;
}