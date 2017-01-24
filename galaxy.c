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

struct galaxy_game_state_t {
	struct coordinates_t spaceship;
	size_t stone_move_count;
	size_t rocket_send_count;
	int score;
};

static struct galaxy_game_state_t galaxy_game_state;

static void fill_row(size_t row, long* const area) {
	size_t colomn;
	for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
		area[row * PLAYGROUND_COLOMNS + colomn] = (rand() % FILL_FACTOR) > 0 ? STONE : 0;
	}
}

static void rocket_miss(long* const area) {
	size_t colomn;
	for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
		if (area[colomn] == ROCKET) area[colomn] = 0;
	}
}

/*move rockets up, but not move from space ship level*/
static void move_rockets(long* const area) {
	size_t row, colomn;
	for (row = 0; row < PLAYGROUND_ROWS - 2; ++row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			if (area[(row + 1) * PLAYGROUND_COLOMNS + colomn] == ROCKET) {
				galaxy_game_state.score += ROCKET_HIT + ROCKET_HIT_BONUS * get_speed();
				area[row * PLAYGROUND_COLOMNS + colomn] += ROCKET;
				area[(row + 1) * PLAYGROUND_COLOMNS + colomn] = 0;
			}
		}
	}
}

static void send_rocket(long* const area) {
	galaxy_game_state.rocket_send_count = 0;
	area[(galaxy_game_state.spaceship.y - 1) * PLAYGROUND_COLOMNS + galaxy_game_state.spaceship.x] += ROCKET;
}

static void move_spaceship(long* const area, enum horizontal_move_t move_derection) {
	struct coordinates_t destination;
	destination.x = galaxy_game_state.spaceship.x + move_derection;
	destination.y = galaxy_game_state.spaceship.y;
	if (!(destination.x < 0 || destination.x >= PLAYGROUND_COLOMNS)) {
		
		area[destination.y * PLAYGROUND_COLOMNS + destination.x] = 1;
		area[galaxy_game_state.spaceship.y * PLAYGROUND_COLOMNS + galaxy_game_state.spaceship.x] = 0;
		galaxy_game_state.spaceship = destination;
	}
}

static void move_stones(long* const area) {
	size_t row, colomn;
	galaxy_game_state.stone_move_count = 0;
	for (row = PLAYGROUND_ROWS - 1; row > 0; --row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			if (area[(row - 1) * PLAYGROUND_COLOMNS + colomn] == STONE) {
				area[row * PLAYGROUND_COLOMNS + colomn] = STONE;
				area[(row - 1) * PLAYGROUND_COLOMNS + colomn] = 0;
			}
		}
	}
}

static int is_game_failed(long* const area) {
	size_t colomn;
	for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
		if (area[(PLAYGROUND_ROWS - 1) * PLAYGROUND_COLOMNS + colomn] == STONE) {
			return 1;
		}
	}
	return 0;
}

int init_galaxy(long** area, long** info_blocs) {
	size_t i;
	*area = (long*)calloc(PLAYGROUND_COLOMNS * PLAYGROUND_ROWS, sizeof(long));
	*info_blocs = (long*)calloc(INFO_BLOCKS_COUNT * INFO_BLOCKS_COUNT, sizeof(long));
	for (i = 0; i <= INIT_ROWS; ++i) {
		fill_row(i, *area);
	}
	galaxy_game_state.score = 0;
	galaxy_game_state.stone_move_count = 0;
	galaxy_game_state.rocket_send_count = ROCKET_SEND_SPEED;
	galaxy_game_state.spaceship.x = 5;
	galaxy_game_state.spaceship.y = PLAYGROUND_ROWS - 1;
	/*Init space ship*/
	(*area)[galaxy_game_state.spaceship.y * PLAYGROUND_COLOMNS + galaxy_game_state.spaceship.x] = 1;
	return 0;
}

int next_step_galaxy(long* const area, long* const info_blocs, const SDL_Keycode* const key_code) {
	rocket_miss(area);
	move_rockets(area);
	if (++galaxy_game_state.stone_move_count > STONE_SPEED) {
		move_stones(area);
		fill_row(0, area);
		galaxy_game_state.score += SURVIVE_STONE_LINE + SURVIVE_STONE_LINE_BONUS * get_speed();
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
	if (++galaxy_game_state.rocket_send_count > ROCKET_SEND_SPEED) {
		send_rocket(area);
	}
	return galaxy_game_state.score;
}