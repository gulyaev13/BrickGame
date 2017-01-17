#pragma once
#ifndef APP_WINDOW_H_INCLUDED
#define APP_WINDOW_H_INCLUDED

#define PLAYGROUND_COLOMNS 10
#define PLAYGROUND_ROWS 20



typedef enum {
	SDL_INIT_EVERYTHING_OK = 0,
	SDL_INIT_ERROR,
	SDL_CREATE_WINDOW_ERROR,
	SDL_GET_SURFACE_ERROR,
} gui_init_t;

int init_gui();

int create_gui();

void render(const int* const area);

void destroy_gui();

void fill_red_test();
#endif