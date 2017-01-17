#include "app_window.h"
#include <stdlib.h>
#include <time.h>

#define CHERRY -1

typedef struct {
	int x;
	int y;
} coordinates_t;

static coordinates_t snake_head;
static coordinates_t direction;
static coordinates_t old_direction;
static int lives;
static int snake_length;

static update_lives(int* const info_blocs) {
	int row;
	for (row = INFO_BLOCKS_COUNT - 1; row >= 0; --row) {
		info_blocs[row * INFO_BLOCKS_COUNT] = lives - (INFO_BLOCKS_COUNT - row - 1) > 0 ? 1 : 0;
	}
}

static void create_cherry(int* area) {
	int rand_value, row, colomn, count = 0;
	srand(time(NULL));
	rand_value = rand() % (PLAYGROUND_COLOMNS * PLAYGROUND_ROWS - snake_length);
	for (row = 0; row < PLAYGROUND_ROWS; ++row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			if (!(area[row * PLAYGROUND_COLOMNS + colomn])) {
				count++;
				if(count == rand_value) area[row * PLAYGROUND_COLOMNS + colomn] = CHERRY;
			}
		}
	}

}

int init_snake(int** area, int** info_blocs) {
	int i;
	*area = (int*)calloc(PLAYGROUND_COLOMNS * PLAYGROUND_ROWS, sizeof(int));
	*info_blocs = (int*)calloc(INFO_BLOCKS_COUNT * INFO_BLOCKS_COUNT, sizeof(int));
	lives = 3;
	snake_length = 5;
	update_lives(*info_blocs);
	snake_head.x = 4;
	snake_head.y = 10;
	/*move up*/
	direction.x = 0;
	direction.y = -1;
	old_direction.x = 0;
	old_direction.y = -1;
	for (i = snake_length; i > 0; --i) {
		int coord = (snake_head.y + (snake_length - i)) * PLAYGROUND_COLOMNS + snake_head.x;
		(*area)[coord] = i;
	}
	create_cherry(*area);
	return 0;
}

int next_step_snake(int* const area, int* const info_blocs) {
	return 0;
}