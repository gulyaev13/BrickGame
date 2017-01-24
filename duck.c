#include "app_window.h"
#include "SDL2/SDL.h"
#include <stdlib.h>
const long easter_image[200] = {
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

static void set_easter_egg(long* const area) {
	size_t row, colomn;
	for (row = 0; row < PLAYGROUND_ROWS; ++row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			area[row * PLAYGROUND_COLOMNS + colomn] = easter_image[row * PLAYGROUND_COLOMNS + colomn];
		}
	}
}
int init_duck(long** area, long** info_blocs) {
	*area = (long*)calloc(PLAYGROUND_COLOMNS * PLAYGROUND_ROWS, sizeof(long));
	*info_blocs = (long*)calloc(INFO_BLOCKS_COUNT * INFO_BLOCKS_COUNT, sizeof(long));
	set_easter_egg(*area);
	return 0;
}

int next_step_duck(long* const area, long* const info_blocs, const SDL_Keycode* const key_code) {
	return -1;
}