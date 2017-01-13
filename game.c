/**
 * game.c -- main file of the pokemon ball game
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SDL.h"
#include "alien.h"
#include "text.h"

// window settings
const int SCREEN_W = 1024;
const int SCREEN_H = 768;

// Acceleration of ship
const int X_ACCELERATION = 1;
const int Y_ACCELERATION = 1;

int COMPLEXITY = 4;
int SCORE = 0;

extern alien ship;
extern int pokemons_count;

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
	img = SDL_LoadBMP("img/yellow_pokemon.bmp");
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
	screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (screen == NULL) {
		fprintf(stderr, "Couldn't set %dx%d video mode: %s\n", 
			SCREEN_W, SCREEN_H, SDL_GetError());
		exit(2);
	}
	SDL_WM_SetCaption("Pokemon Ball", NULL);

	init_fonts();

	init_alien_surfaces();
}

void refresh() {
	SDL_Rect dest;
	
	dest.x = dest.y = 0; 
	dest.w = SCREEN_W, dest.h = SCREEN_H;

	if (SDL_FillRect(screen, &dest, 0) < 0) {
		fprintf(stderr, "refresh() : %s\n", SDL_GetError());
		exit(1);
	}
}

void draw_all() {
	draw_all_aliens();
	draw_score();
	if (SDL_Flip(screen) < 0) {
		fprintf(stderr, "draw_all() | SDL_Flip : %s\n", SDL_GetError());
	}
}

int main() {
	srand(time(NULL));
	init_all();
	int done = 0;
	
	// ship's start
	ship = create_alien(SHIP, 437, 600);
		
	while (!done) {
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_LEFT:
							ship.x_acc = -X_ACCELERATION;
							ship.x_vel = 0;
							break;
						case SDLK_RIGHT:
							ship.x_acc = X_ACCELERATION;
							ship.x_vel = 0;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
						case SDLK_LEFT:
							ship.x_acc = X_ACCELERATION;
							break;
						case SDLK_RIGHT:
							ship.x_acc = -X_ACCELERATION;
							break;
						default:
							break;
					}
					break;
				case SDL_QUIT: 
					done = 1;
					break;
				default:
					break;
			}
		}
		refresh();
		draw_all();
		if (check_collisions_with_pokemons()) {
			fprintf(stderr, "YOU DON'T WIN!\n");
			SDL_Delay(300);
			exit(1);
		}
		clean_pokemons();
		if (pokemons_count < COMPLEXITY && rand() < 10000000 * COMPLEXITY)
			generate_pokemon();
		SDL_Delay(9);
	}
	return 0;
}