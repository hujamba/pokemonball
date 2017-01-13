#ifndef TEXT_H
#define TEXT_H

void init_fonts();

void write_text(int x, int y, char text[100], int sz, int r, int g, int b);

void draw_score();

void draw_start_message();

void draw_game_over();

#endif