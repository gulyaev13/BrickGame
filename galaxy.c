#include "app_window.h"
#include "menu.h"
#include "structs.h"
#include "SDL2/SDL.h"
#include <stdlib.h>
#include <time.h>

#define FILL_FACTOR 4
#define INIT_ROWS 5

#define ROCKET 1
#define STONE -1

#define STONE_SPEED 16
#define ROCKET_SEND_SPEED 3

#define ROCKET_HIT 5
#define ROCKET_HIT_BONUS 1
#define SURVIVE_STONE_LINE 100
#define SURVIVE_STONE_LINE_BONUS 50

static coordinates_t spaceship;
static int stone_move_count;
static int rocket_send_count;
static int score;

static void fill_row(int row, int* const area) {
	int colomn;
	for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
		area[row * PLAYGROUND_COLOMNS + colomn] = (rand() % FILL_FACTOR) > 0 ? STONE : 0;
	}
}

static void rocket_miss(int* const area) {
	int colomn;
	for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
		if (area[colomn] == ROCKET) area[colomn] = 0;
	}
}

/*move rockets up, but not move from space ship level*/
static void move_rockets(int* const area) {
	int row, colomn;
	for (row = 0; row < PLAYGROUND_ROWS - 2; ++row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			if (area[(row + 1) * PLAYGROUND_COLOMNS + colomn] == ROCKET) {
				score += ROCKET_HIT + ROCKET_HIT_BONUS * get_speed();
				area[row * PLAYGROUND_COLOMNS + colomn] += ROCKET;
				area[(row + 1) * PLAYGROUND_COLOMNS + colomn] = 0;
			}
		}
	}
}

static void send_rocket(int* const area) {
	rocket_send_count = 0;
	area[(spaceship.y - 1) * PLAYGROUND_COLOMNS + spaceship.x] += ROCKET;
}

static void move_spaceship(int* const area, horizontal_move_t move_derection) {
	coordinates_t destination;
	destination.x = spaceship.x + move_derection;
	destination.y = spaceship.y;
	if (!(destination.x < 0 || destination.x >= PLAYGROUND_COLOMNS)) {
		
		area[destination.y * PLAYGROUND_COLOMNS + destination.x] = 1;
		area[spaceship.y * PLAYGROUND_COLOMNS + spaceship.x] = 0;
		spaceship = destination;
	}
}

static void move_stones(int* const area) {
	int row, colomn;
	stone_move_count = 0;
	for (row = PLAYGROUND_ROWS - 1; row > 0; --row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			if (area[(row - 1) * PLAYGROUND_COLOMNS + colomn] == STONE) {
				area[row * PLAYGROUND_COLOMNS + colomn] = STONE;
				area[(row - 1) * PLAYGROUND_COLOMNS + colomn] = 0;
			}
		}
	}
}

static int is_game_failed(int* const area) {
	int colomn;
	for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
		if (area[(PLAYGROUND_ROWS - 1) * PLAYGROUND_COLOMNS + colomn] == STONE) {
			return 1;
		}
	}
	return 0;
}

int init_galaxy(int** area, int** info_blocs) {
	int i;
	*area = (int*)calloc(PLAYGROUND_COLOMNS * PLAYGROUND_ROWS, sizeof(int));
	*info_blocs = (int*)calloc(INFO_BLOCKS_COUNT * INFO_BLOCKS_COUNT, sizeof(int));
	for (i = 0; i <= INIT_ROWS; ++i) {
		fill_row(i, *area);
	}
	score = 0;
	stone_move_count = 0;
	rocket_send_count = ROCKET_SEND_SPEED;
	spaceship.x = 5;
	spaceship.y = PLAYGROUND_ROWS - 1;
	/*Init space ship*/
	(*area)[spaceship.y * PLAYGROUND_COLOMNS + spaceship.x] = 1;
	return 0;
}

int next_step_galaxy(int* const area, int* const info_blocs, const SDL_Keycode* const key_code) {
	rocket_miss(area);
	move_rockets(area);
	if (++stone_move_count > STONE_SPEED) {
		move_stones(area);
		fill_row(0, area);
		score += SURVIVE_STONE_LINE + SURVIVE_STONE_LINE_BONUS * get_speed();
	}
	if (is_game_failed(area)) return -1;
	if (key_code != NULL) {
		switch (*key_code) {
		case SDLK_LEFT:
			move_spaceship(area, LEFT);
			break;
		case SDLK_RIGHT:
			move_spaceship(area, RIGHT);
			break;
		case SDLK_SPACE:
			send_rocket(area);
			break;
		}
	}
	if (++rocket_send_count > ROCKET_SEND_SPEED) {
		send_rocket(area);
	}
	return score;
}