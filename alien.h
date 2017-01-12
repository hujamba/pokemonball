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
	int r;            // radius
} alien;

alien create_alien(int type, int x, int y);

void draw_alien(alien *a);

int has_collided(alien *a, alien *b);

void update_alien(alien *a);

void draw_aliens(alien *a, int cnt);

void draw_all();

void swap_aliens(alien *a, alien *b);

/*
 * Pokemons!
 */
int check_collisions_with_pokemons();

void delete_pokemon(int id);

void clean_pokemons();

void generate_pokemon();

void generate_pokemons(int add);

#endif