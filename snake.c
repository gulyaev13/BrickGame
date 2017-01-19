#include "app_window.h"
#include "menu.h"
#include "structs.h"
#include <stdlib.h>
#include <time.h>

#define CHERRY -1
#define CHERRY_ADD_SPEED 20
#define MAX_CHERRY_COUNT 2

typedef enum {
	STEP_FAILED = 0,
	STEP_TRUE,
	EAT_CHERRY
} step_state_t;

static coordinates_t snake_head;
static coordinates_t direction;
static coordinates_t new_direction;
static int lives;
static int snake_length;
static int score;
static int cherry_interval;
static int cherry_count;
static update_lives(int* const info_blocs) {
	int row;
	for (row = INFO_BLOCKS_COUNT - 1; row >= 0; --row) {
		info_blocs[row * INFO_BLOCKS_COUNT] = lives - (INFO_BLOCKS_COUNT - row - 1) > 0 ? 1 : 0;
	}
}

static void create_cherry(int* const area) {
	int rand_value, row, colomn, count = 0;
	if (cherry_count < MAX_CHERRY_COUNT) {
		srand((unsigned int)time(NULL));
		rand_value = rand() % (PLAYGROUND_COLOMNS * PLAYGROUND_ROWS - snake_length);
		for (row = 0; row < PLAYGROUND_ROWS; ++row) {
			for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
				if (!(area[row * PLAYGROUND_COLOMNS + colomn])) {
					count++;
					if (count == rand_value) area[row * PLAYGROUND_COLOMNS + colomn] = CHERRY;
				}
			}
		}
		cherry_count++;
	}
}

static coordinates_t coordinates_sum(coordinates_t a, coordinates_t b) {
	coordinates_t c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}

static int is_out_of_playground(coordinates_t a) {
	if (a.x < 0 || a.x >= PLAYGROUND_COLOMNS) return 1;
	if (a.y < 0 || a.y >= PLAYGROUND_ROWS) return 1;
	return 0;
}

static int is_eat_itself(coordinates_t a, int* const area) {
	if (area[a.y * PLAYGROUND_COLOMNS + a.x] > 0) return 1;
	return 0;
}

static int is_eat_cherry(coordinates_t a, int* const area) {
	if (area[a.y * PLAYGROUND_COLOMNS + a.x] < 0) return 1;
	return 0;
}

static int step(int* const area) {
	coordinates_t destination = coordinates_sum(snake_head, direction);
	if (is_out_of_playground(destination)) return STEP_FAILED;
	if (is_eat_itself(destination, area)) return STEP_FAILED;
	snake_head = destination;
	if (is_eat_cherry(destination, area)) {
		snake_length++;
		area[snake_head.y * PLAYGROUND_COLOMNS + snake_head.x] = snake_length;
		return EAT_CHERRY;
	}
	area[snake_head.y * PLAYGROUND_COLOMNS + snake_head.x] = snake_length + 1;
	return STEP_TRUE;
}

static void move_snake(int* const area) {
	int row, colomn;
	for (row = 0; row < PLAYGROUND_ROWS; ++row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			if (area[row * PLAYGROUND_COLOMNS + colomn] > 0) (area[row * PLAYGROUND_COLOMNS + colomn])--;
		}
	}
}

static int is_not_inverse(coordinates_t a, coordinates_t b) {
	/*if derection is inverse return 0*/
	return (a.x + b.x) || (a.y + b.y);
}

static void re_init_snake(int lives_num, int* const area, int* const info_blocs) {
	int i, row, colomn;
	for (row = 0; row < PLAYGROUND_ROWS; ++row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			area[row * PLAYGROUND_COLOMNS + colomn] = 0;
		}
	}
	cherry_interval = 0;
	cherry_count = 0;
	lives = lives_num;
	snake_length = 5;
	score = 0;
	update_lives(info_blocs);
	snake_head.x = 4;
	snake_head.y = 10;
	/*move up*/
	direction.x = 0;
	direction.y = -1;
	new_direction.x = 0;
	new_direction.y = -1;
	for (i = snake_length; i > 0; --i) {
		area[(snake_head.y + (snake_length - i)) * PLAYGROUND_COLOMNS + snake_head.x] = i;
	}
	create_cherry(area);
}

int next_step_snake(int* const area, int* const info_blocs, const SDL_Keycode* const key_code) {
	step_state_t step_state;
	new_direction.x = 0;
	new_direction.y = 0;
	if (key_code != NULL) {
		switch (*key_code) {
		case SDLK_DOWN:
			new_direction.y = 1;
			break;
		case SDLK_UP:
			new_direction.y = -1;
			break;
		case SDLK_LEFT:
			new_direction.x = -1;
			break;
		case SDLK_RIGHT:
			new_direction.x = 1;
			break;
		}
		if (new_direction.x || new_direction.y) {
			if (is_not_inverse(direction, new_direction)) {
				direction = new_direction;
			}
		}

	}
	step_state = step(area);
	if (step_state == STEP_FAILED) {
		if (--lives == 0) return -1;
		re_init_snake(lives, area, info_blocs);
	}
	if (step_state == STEP_TRUE) {
		move_snake(area);
		score += 5 + 1 * get_speed();
	}
	if (step_state == EAT_CHERRY) {
		score += 100 + 50 * get_speed();
		cherry_count--;
		create_cherry(area);
		cherry_interval = 0;
	}
	if (++cherry_interval > CHERRY_ADD_SPEED) {
		create_cherry(area);
		cherry_interval = 0;
	}
	return score;
}

int init_snake(int** area, int** info_blocs) {
	*area = (int*)calloc(PLAYGROUND_COLOMNS * PLAYGROUND_ROWS, sizeof(int));
	*info_blocs = (int*)calloc(INFO_BLOCKS_COUNT * INFO_BLOCKS_COUNT, sizeof(int));
	re_init_snake(3, *area, *info_blocs);
	return 0;
}