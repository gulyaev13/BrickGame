#pragma once
#ifndef APP_WINDOW_H_INCLUDED
#define APP_WINDOW_H_INCLUDED

#define PLAYGROUND_COLOMNS 10
#define PLAYGROUND_ROWS 20

#define BRICK_SIZE 13
#define BRICK_SIZE_WITH_BORDER 14

#define BORDER_START_X 2
#define BORDER_START_Y 2

/*Colors*/
#define BACKGROUND_COLOR 0xD3D3D3
#define INDIGO_COLOR 0x4B0082
#define RED_COLOR 0xFF0000
#define BLACK_COLOR 0x000000
#define DARK_GREY_COLOR 0x808080

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