#include <stdio.h>
#include "app_window.h"
#include "menu.h"

#define SPEED_FACTOR 35

typedef enum {
	GAME_START = 0,
	GAME_OVER = -1
} game_state_t;

static int* area = NULL;
static int* info_blocs = NULL;

void free_areas() {
	free(area);
	free(info_blocs);
	area = NULL;
	info_blocs = NULL;
}
static void animation(game_state_t state) {
	int row, colomn;
	free_areas();
	area = (int*)calloc(PLAYGROUND_COLOMNS * PLAYGROUND_ROWS, sizeof(int));
	info_blocs = (int*)calloc(INFO_BLOCKS_COUNT * INFO_BLOCKS_COUNT, sizeof(int));
	render(area, info_blocs);
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
	free_areas();
}

static void pre_game_pause() {
	SDL_Event event;
	int working = 1;
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
		}
	}
}

static void game_pause() {
	SDL_Event event;
	int working = 1;
	set_pause_text(TEXT_SHOW_ON);
	while (working) {
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			destroy_gui();
			exit(0);
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym){
			case SDLK_RETURN:
				working = 0;
				break;
			}
		}
	}
	set_pause_text(TEXT_SHOW_OFF);
}

int game_play() {
	SDL_Event event;
	int working = 1;
	int score;
	set_info_text(GAME_TEXT);
	animation(GAME_START);
	init_game(&area, &info_blocs);
	render(area, info_blocs);
	pre_game_pause();
	do {
		if (SDL_WaitEventTimeout(&event, 450 - SPEED_FACTOR * get_speed())) {
			switch (event.type) {
			case SDL_QUIT:
				working = 0;
				//destroy_gui();
				//exit(0);
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_RETURN:
					game_pause();
					break;
				case SDLK_ESCAPE:
					working = -1;
					break;
				default:
					score = game_next_step(area, info_blocs, &(event.key.keysym.sym));
					break;
				}
			}
		}
		else {
			score = game_next_step(area, info_blocs, NULL);
		}
		if (!working) break;
		render(area, info_blocs);
		if (score > 0) {
			set_score_num(score);
			if (score > get_hi_score()) {
				set_hi_score(score);
				set_hi_score_num(score);
			}
			
		} 
		else if (score == 0) {
			set_score_num(score);
			pre_game_pause();
		}
		else {
			working = -1;
		}
		if (working < 0) set_score_num(0); 
	} while (working > 0);
	animation(GAME_OVER);
	set_info_text(MENU_TEXT);
	return working;
}