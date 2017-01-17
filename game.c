#include <stdio.h>
#include "app_window.h"
#include "menu.h"
typedef enum {
	GAME_START = 0,
	GAME_OVER = -1
} game_state_t;

static int* area = NULL;
static int* info_blocs = NULL;

static void animation(game_state_t state) {
	int row, colomn;
	area = (int*)calloc(PLAYGROUND_COLOMNS * PLAYGROUND_ROWS, sizeof(int));
	if(state) set_game_over_text(TEXT_SHOW_ON);
	for (row = PLAYGROUND_ROWS - 1; row >= 0; --row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			area[row * PLAYGROUND_COLOMNS + colomn] = 1;
		}
		render(area, NULL);
		SDL_Delay(80);
	}
	for (row = 0; row < PLAYGROUND_ROWS; ++row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			area[row * PLAYGROUND_COLOMNS + colomn] = 0;
		}
		render(area, NULL);
		SDL_Delay(80);
	}
	if (state) set_game_over_text(TEXT_SHOW_OFF);
	free(area);
	area = NULL;
}

void game_play() {
	SDL_Event event;
	int working = 1;
	animation(GAME_START);
	init_game(&area, &info_blocs);
	render(area, info_blocs);
	while (working) {
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			destroy_gui();
			exit(0);
			break;
		case SDL_KEYDOWN:
			working = 0;
			break;
		default:
			break;
		}
	}
	animation(GAME_OVER);
}

void free_area() {
	free(area);
	free(info_blocs);
}