#include "app_window.h"
#include "menu.h"
#include "structs.h"
#include "SDL2/SDL.h"
#include <stdlib.h>
#include <time.h>

#define CHERRY -1
#define CHERRY_ADD_SPEED 20
#define MAX_CHERRY_COUNT 2

#define MOVE_SCORE 5
#define MOVE_SCORE_BONUS 1
#define EAT_CHERRY_SCORE 100
#define EAT_CHERRY_BONUS 50

enum step_state_t {
	STEP_FAILED = 0,
	STEP_TRUE,
	EAT_CHERRY
};
struct snake_game_state_t {
	struct coordinates_t snake_head;
	struct coordinates_t direction;
	struct coordinates_t new_direction;
	size_t lives;
	size_t snake_length;
	int score;
	size_t cherry_interval;
	size_t cherry_count;
};

static struct snake_game_state_t snake_game_state;

static void update_lives(long* const info_blocs) {
	size_t row;
	for (row = INFO_BLOCKS_COUNT; row > 0; --row) {
		info_blocs[(row - 1) * INFO_BLOCKS_COUNT] = (long)snake_game_state.lives - (INFO_BLOCKS_COUNT - (long)row) > 0 ? 1 : 0;
	}
}

static void create_cherry(long* const area) {
	size_t rand_value, row, colomn, count = 0;
	if (snake_game_state.cherry_count < MAX_CHERRY_COUNT) {
		srand((unsigned int)time(NULL));
		rand_value = rand() % (PLAYGROUND_COLOMNS * PLAYGROUND_ROWS - snake_game_state.snake_length);
		for (row = 0; row < PLAYGROUND_ROWS; ++row) {
			for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
				if (!(area[row * PLAYGROUND_COLOMNS + colomn])) {
					count++;
					if (count == rand_value) area[row * PLAYGROUND_COLOMNS + colomn] = CHERRY;
				}
			}
		}
		snake_game_state.cherry_count++;
	}
}

static struct coordinates_t coordinates_sum(struct coordinates_t a, struct coordinates_t b) {
	struct coordinates_t c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}

static int is_out_of_playground(struct coordinates_t a) {
	if (a.x < 0 || a.x >= PLAYGROUND_COLOMNS) return 1;
	if (a.y < 0 || a.y >= PLAYGROUND_ROWS) return 1;
	return 0;
}

static int is_eat_itself(struct coordinates_t a, long* const area) {
	if (area[a.y * PLAYGROUND_COLOMNS + a.x] > 0) return 1;
	return 0;
}

static int is_eat_cherry(struct coordinates_t a, long* const area) {
	if (area[a.y * PLAYGROUND_COLOMNS + a.x] < 0) return 1;
	return 0;
}

static int step(long* const area) {
	struct coordinates_t destination = coordinates_sum(snake_game_state.snake_head, snake_game_state.direction);
	if (is_out_of_playground(destination)) return STEP_FAILED;
	if (is_eat_itself(destination, area)) return STEP_FAILED;
	snake_game_state.snake_head = destination;
	if (is_eat_cherry(destination, area)) {
		snake_game_state.snake_length++;
		area[snake_game_state.snake_head.y * PLAYGROUND_COLOMNS + snake_game_state.snake_head.x] = (long)snake_game_state.snake_length;
		return EAT_CHERRY;
	}
	area[snake_game_state.snake_head.y * PLAYGROUND_COLOMNS + snake_game_state.snake_head.x] = (long)snake_game_state.snake_length + 1;
	return STEP_TRUE;
}

static void move_snake(long* const area) {
	size_t row, colomn;
	for (row = 0; row < PLAYGROUND_ROWS; ++row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			if (area[row * PLAYGROUND_COLOMNS + colomn] > 0) (area[row * PLAYGROUND_COLOMNS + colomn])--;
		}
	}
}

static int is_not_inverse(struct coordinates_t a, struct coordinates_t b) {
	/*if derection is inverse return 0*/
	return (a.x + b.x) || (a.y + b.y);
}

static void re_init_snake(size_t lives_num, long* const area, long* const info_blocs) {
	size_t i, row, colomn;
	for (row = 0; row < PLAYGROUND_ROWS; ++row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			area[row * PLAYGROUND_COLOMNS + colomn] = 0;
		}
	}
	snake_game_state.cherry_interval = 0;
	snake_game_state.cherry_count = 0;
	snake_game_state.lives = lives_num;
	snake_game_state.snake_length = 5;
	snake_game_state.score = 0;
	update_lives(info_blocs);
	snake_game_state.snake_head.x = 4;
	snake_game_state.snake_head.y = 10;
	/*move up*/
	snake_game_state.direction.x = 0;
	snake_game_state.direction.y = -1;
	snake_game_state.new_direction.x = 0;
	snake_game_state.new_direction.y = -1;
	for (i = snake_game_state.snake_length; i > 0; --i) {
		area[(snake_game_state.snake_head.y + (snake_game_state.snake_length - i)) * PLAYGROUND_COLOMNS + snake_game_state.snake_head.x] = (long)i;
	}
	create_cherry(area);
}

int next_step_snake(long* const area, long* const info_blocs, const SDL_Keycode* const key_code) {
	enum step_state_t step_state;
	snake_game_state.new_direction.x = 0;
	snake_game_state.new_direction.y = 0;
	if (key_code != NULL) {
		switch (*key_code) {
		case SDLK_DOWN:
			snake_game_state.new_direction.y = 1;
			break;
		case SDLK_UP:
			snake_game_state.new_direction.y = -1;
			break;
		case SDLK_LEFT:
			snake_game_state.new_direction.x = -1;
			break;
		case SDLK_RIGHT:
			snake_game_state.new_direction.x = 1;
			break;
		}
		if (snake_game_state.new_direction.x || snake_game_state.new_direction.y) {
			if (is_not_inverse(snake_game_state.direction, snake_game_state.new_direction)) {
				snake_game_state.direction = snake_game_state.new_direction;
			}
		}

	}
	step_state = step(area);
	if (step_state == STEP_FAILED) {
		if (--snake_game_state.lives == 0) return -1;
		re_init_snake(snake_game_state.lives, area, info_blocs);
	}
	if (step_state == STEP_TRUE) {
		move_snake(area);
		snake_game_state.score += MOVE_SCORE + MOVE_SCORE_BONUS * get_speed();
	}
	if (step_state == EAT_CHERRY) {
		snake_game_state.score += EAT_CHERRY_SCORE + EAT_CHERRY_BONUS * get_speed();
		snake_game_state.cherry_count--;
		create_cherry(area);
		snake_game_state.cherry_interval = 0;
	}
	if (++snake_game_state.cherry_interval > CHERRY_ADD_SPEED) {
		create_cherry(area);
		snake_game_state.cherry_interval = 0;
	}
	return snake_game_state.score;
}

int init_snake(long** area, long** info_blocs) {
	*area = (long*)calloc(PLAYGROUND_COLOMNS * PLAYGROUND_ROWS, sizeof(long));
	*info_blocs = (long*)calloc(INFO_BLOCKS_COUNT * INFO_BLOCKS_COUNT, sizeof(long));
	re_init_snake(3, *area, *info_blocs);
	return 0;
}