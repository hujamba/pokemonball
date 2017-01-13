#include "SDL.h"
#include "SDL_ttf.h"

extern SDL_Surface *screen;
extern int COMPLEXITY;
extern int SCORE;

char str[100];

void init_fonts() {
	if (TTF_Init() < 0) {
		fprintf(stderr, "%s\n", TTF_GetError());
		exit(2);
	}
}

void write_text(int x, int y, char text[100], int sz, int r, int g, int b) {
	SDL_Color clr;
	clr.r = r;
	clr.g = g;
	clr.b = b;
	
	TTF_Font *fnt = TTF_OpenFont("fonts/NOCONSEQUENCE.ttf", sz);
	if (fnt == NULL) {
		fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
	}
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_Surface *text_surface = TTF_RenderText_Blended(fnt, text, clr);
	SDL_BlitSurface(text_surface, NULL, screen, &dest);
	SDL_FreeSurface(text_surface);
	TTF_CloseFont(fnt);
}

void draw_score() {
	write_text(830, 0, "SCORE", 40, 255, 0, 0);
	sprintf(str, "%05d", SCORE);
	write_text(830, 30, str, 40, 244, 244, 0);
	
	write_text(830, 70, "LEVEL", 40, 255, 0, 0);
	sprintf(str, "%d", COMPLEXITY);
	write_text(830, 100, str, 40, 244, 244, 0);
}

void draw_start_message() {
	write_text(330, 320, "Press Enter", 42, 244, 244, 0);
	write_text(250, 350, "to start the game!", 42, 244, 244, 0);
}

void draw_game_over() {
	write_text(250, 370, "GAME OVER!", 72, 244, 244, 0);
}