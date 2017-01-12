#include "SDL.h"
#include "alien.h"
#include "helpers.h"

extern SDL_Surface *screen;
extern SDL_Surface *aliens[2];

extern const int SCREEN_W;
extern const int SCREEN_H;


int MIN_VEL_X[2] = { -10, -10 };
int MAX_VEL_X[2] = { 10, 10 };

int MIN_VEL_Y[2] = { -15, -15 };
int MAX_VEL_Y[2] = { 15, 15 };


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

// handler of alien's movement
void update_alien(alien *a) {
	int type = a->type;

	switch (type) {
		case SHIP:
			update_alien_ship(a);
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