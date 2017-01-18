#pragma once
#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

typedef enum {
	INIT_MENU_OK = 0,
	INIT_MENU_ERROR
} menu_init_code;
typedef struct menuelement_t {
	const int* menu_image;
	int hi_score;
	int(*init)(int**, int**);
	int(*next_step)(int* const, int* const, const SDL_Keycode* const);
	/*return score or -1 if gameover*/
} menuelement_t;

typedef struct menu_t {
	int count;
	int capacity;
	int choice;
	int speed;
	menuelement_t *menuelement;
} menu_t;

int init_menu();

void choose_menu_item();

int init_game(int** area, int** info_blocs);

int game_next_step(int* const area, int* const info_blocs, const SDL_Keycode* const key_code);

int get_speed();

void set_speed(int new_speed);

int get_hi_score();

void set_hi_score(int new_hi_score);
#endif