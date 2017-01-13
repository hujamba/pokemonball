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

int COMPLEXITY = 1;
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
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
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

void screen_flip() {
	if (SDL_Flip(screen) < 0) {
		fprintf(stderr, "SDL_Flip : %s\n", SDL_GetError());
		exit(1);
	}
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
	refresh();
	draw_all_aliens();
	draw_score();
}

Uint32 up_level(Uint32 interval, void *param) {
	COMPLEXITY += 1;
	fprintf(stderr, "UP LEVEL!\n");
	return (interval);
}

void reset_all() {
	SCORE = 0;
	COMPLEXITY = 1;
	pokemons_count = 0;
}

int wait_enter() {
	SDL_Event event;
	while (1) {
		SDL_WaitEvent(&event);
		SDL_PollEvent(&event);
		switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case 13: // SDLK_KP_ENTER -- on linux doesn't work!(code 10)
						return 1;
					case SDLK_ESCAPE:
						return 0;
					default:
						break;
				}
				break;
			case SDL_QUIT:
				return 1;
			default:
				break;
		}
	}
	return 0;
}

SDL_TimerID score_timer_id;

void start_game() {
	reset_all();
	// ship's start
	refresh();
	ship = create_alien(SHIP, 437, 600);
	score_timer_id = SDL_AddTimer(5000, up_level, NULL);
	draw_all();
	screen_flip();
}

void finish_game() {
	SDL_RemoveTimer(score_timer_id);
	draw_game_over();
	screen_flip();		
}

int game() {
	int done = 0;
	
	start_game();

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
						case SDLK_ESCAPE:
							done = 1;
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
		draw_all();
		if (check_collisions_with_pokemons()) {
			finish_game();
			return wait_enter();
		}
		clean_pokemons();
		if (pokemons_count < COMPLEXITY && rand() < 10000000 * COMPLEXITY)
			generate_pokemon();
		screen_flip();
		SDL_Delay(9);
	}
	finish_game();
	return 0;
}

int start_screen() {
	refresh();
	draw_start_message();
	screen_flip();
	return wait_enter();;
}

int main() {
	srand(time(NULL));
	init_all();
	if (start_screen())
		while (game());
	return 0;
}