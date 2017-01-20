#pragma once
#ifndef APP_WINDOW_H_INCLUDED
#define APP_WINDOW_H_INCLUDED

#define PLAYGROUND_COLOMNS 10
#define PLAYGROUND_ROWS 20
#define INFO_BLOCKS_COUNT 4

typedef enum {
	SDL_INIT_EVERYTHING_OK = 0,
	SDL_INIT_ERROR,
	SDL_CREATE_WINDOW_ERROR,
	SDL_GET_SURFACE_ERROR
} gui_init_t;

typedef enum text_show_t {
	TEXT_SHOW_OFF = 0,
	TEXT_SHOW_ON
} text_show_t;

typedef enum {
	GAME_TEXT = 0,
	MENU_TEXT
} info_text_state_t;

int init_gui();

int create_gui();

void render(const int* const area, const int* const info_blocs);

void destroy_gui();

void fill_red_test();

void set_hi_score_num(int hi_score);

void set_score_num(int score);

void set_speed_num(int speed);

void set_pause_text(text_show_t text_show);

void set_game_over_text(text_show_t text_show);

void set_info_text(info_text_state_t info_text_state);
#endif