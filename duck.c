#include "app_window.h"
#include "SDL2/SDL.h"
#include <stdlib.h>
const int easter_image[200] = {
	1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 1, 0, 0, 1, 0, 0,
	1, 0, 0, 0, 1, 0, 0, 1, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 1, 0, 0,
	1, 0, 0, 0, 1, 0, 0, 1, 0, 0,
	1, 0, 0, 0, 1, 0, 0, 1, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
	0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 1, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 1, 0, 1, 1,
	1, 0, 0, 0, 0, 0, 1, 1, 0, 1,
	1, 1, 1, 1, 0, 0, 1, 0, 0, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 0, 0, 0, 0, 1, 1, 0,
	1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
	1, 0, 0, 1, 0, 0, 1, 1, 1, 1,
	1, 0, 0, 1, 0, 0, 1, 0, 0, 1
};

static void set_easter_egg(int* const area) {
	size_t row, colomn;
	for (row = 0; row < PLAYGROUND_ROWS; ++row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			area[row * PLAYGROUND_COLOMNS + colomn] = easter_image[row * PLAYGROUND_COLOMNS + colomn];
		}
	}

}
int init_duck(int** area, int** info_blocs) {
	*area = (int*)calloc(PLAYGROUND_COLOMNS * PLAYGROUND_ROWS, sizeof(int));
	*info_blocs = (int*)calloc(INFO_BLOCKS_COUNT * INFO_BLOCKS_COUNT, sizeof(int));
	set_easter_egg(*area);
	return 0;
}

int next_step_duck(int* const area, int* const info_blocs, const SDL_Keycode* const key_code) {
	return -1;
}