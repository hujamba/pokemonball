#include "SDL.h"
#include "alien.h"
#include "helpers.h"

#define MAX_POKEMONS 255

extern SDL_Surface *screen;
extern SDL_Surface *aliens[2];

extern const int SCREEN_W;
extern const int SCREEN_H;

extern int COMPLEXITY;

alien ship, pokemons[MAX_POKEMONS];
int pokemons_count;

int MIN_VEL_X[2] = { -10, -5 };
int MAX_VEL_X[2] = { 10, 5 };

int MIN_VEL_Y[2] = { -15, -9 };
int MAX_VEL_Y[2] = { 15, 9 };


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

// handler only for player's ship
static void update_alien_ship(alien *a) {
	if (a->x_acc < 0) {
		if (a->x_vel <= 0)
			a->x_vel = max(MIN_VEL_X[SHIP], a->x_vel + a->x_acc);
		else {
			a->x_vel = max(0, a->x_vel + a->x_acc);
			if (a->x_vel == 0)
				a->x_acc = 0;
		}
	} else {
		if (a->x_vel >= 0)
			a->x_vel = min(MAX_VEL_X[SHIP], a->x_vel + a->x_acc);
		else {
			a->x_vel = min(0, a->x_vel + a->x_acc);
			if (a->x_vel == 0)
				a->x_acc = 0;
		}
	}
	a->x = min(SCREEN_W - aliens[SHIP]->w, max(0, a->x + a->x_vel));
	a->y = min(SCREEN_H - aliens[SHIP]->h, max(0, a->y + a->y_vel));		
}

static void update_alien_pokemon(alien *a) {
	a->x_vel = max(MIN_VEL_X[POKEMON], min(MAX_VEL_X[POKEMON], a->x_vel + a->x_acc));
	a->y_vel = max(MIN_VEL_Y[POKEMON], min(MAX_VEL_Y[POKEMON], a->y_vel + a->y_acc));

	a->x += a->x_vel;
	a->y += a->y_vel;
}

// handler of alien's movement
void update_alien(alien *a) {
	int type = a->type;

	switch (type) {
		case SHIP:
			update_alien_ship(a);
			break;
		case POKEMON:
			update_alien_pokemon(a);
			break;
		default:
			break;
	}
}

void draw_alien(alien *a) {
	SDL_Rect src, dest;

	update_alien(a);

	src.x = src.y = 0;
	src.w = aliens[a->type]->w, src.h = aliens[a->type]->h;

	dest.x = a->x, dest.y = a->y;
	dest.w = aliens[a->type]->w, dest.h = aliens[a->type]->h;

	if (SDL_BlitSurface(aliens[a->type], &src, screen, &dest)) {
		fprintf(stderr, "draw_alien() : %s\n", SDL_GetError());
		exit(1);
	}
}

void draw_aliens(alien *a, int cnt) {
	for (int i = 0; i < cnt; i++) {
		draw_alien(a++);
	}
}

void draw_all() {
	draw_alien(&ship);
	draw_aliens(pokemons, pokemons_count);
	if (SDL_Flip(screen) < 0) {
		fprintf(stderr, "draw_all() | SDL_Flip : %s\n", SDL_GetError());
	}
}

// check collision
int has_collided(alien *a, alien *b) {
	int ax = a->x + aliens[a->type]->w / 2;
	int ay = a->y + aliens[a->type]->h / 2;

	int bx = b->x + aliens[b->type]->w / 2;
	int by = b->y + aliens[b->type]->h / 2;

	int x = (ax - bx);
	int y = (ay - by);

	return (x * x + y * y < (a->r + b->r) * (a->r + b->r));
}

void swap_aliens(alien *a, alien *b) {
	alien c;
	c = *a;
	*a = *b;
	*b = c;
}

int check_collisions_with_pokemons() {
	for (int i = 0; i < pokemons_count; i++) {
		if (has_collided(&ship, &pokemons[i]))
			return 1;
	}
	return 0;
}

void delete_pokemon(int id) {
	fprintf(stderr, "pokemons : %d\n", pokemons_count);
	pokemons_count--;
	swap_aliens(&pokemons[id], &pokemons[pokemons_count]);
}

void clean_pokemons() {
	int i;
	for (i = 0; i < pokemons_count;) {
		if (pokemons[i].y > SCREEN_H 
			|| pokemons[i].x > SCREEN_W
			|| pokemons[i].y < -aliens[POKEMON]->h 
			|| pokemons[i].x < -aliens[POKEMON]->w)
			delete_pokemon(i);
		else
			i++;
	}
}

void generate_pokemon() {
	alien p = create_alien(POKEMON, rand() % (SCREEN_W - aliens[POKEMON]->w), 0);
	p.y_acc = (rand() & 1) + 1;
	p.y_vel = 1;
	//p.x_vel = rand() % 5 - 2;
	pokemons[pokemons_count] = p;
	pokemons_count++;
}

void generate_pokemons(int add) {
	while (add-- && pokemons_count < MAX_POKEMONS)
		generate_pokemon();
}