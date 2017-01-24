#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "SDL2/SDL.h"
enum menu_init_code {
	INIT_MENU_OK = 0,
	INIT_MENU_ERROR
};

int init_menu(void);

void free_menu(void);

void choose_menu_item(void);

int init_game(int** area, int** info_blocs);

int game_next_step(int* const area, int* const info_blocs, const SDL_Keycode* const key_code);

int get_speed(void);

void set_speed(int new_speed);

int get_hi_score(void);

void set_hi_score(int new_hi_score);
#endif