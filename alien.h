#ifndef ALIEN_H
#define ALIEN_H

enum alien_type {
	SHIP,
	POKEMON
};

typedef struct _alien {
	int type;
	int x, y;
	int x_vel, y_vel; // velocity
	int x_acc, y_acc; // acceleration
	int r; // radius
} alien;

alien create_alien(int type, int x, int y);

void draw_alien(alien *a);

#endif