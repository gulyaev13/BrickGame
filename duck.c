#include "app_window.h"
int init_duck(int** area, int** info_blocs) {
	*area = (int*)calloc(PLAYGROUND_COLOMNS * PLAYGROUND_ROWS, sizeof(int));
	*info_blocs = (int*)calloc(INFO_BLOCKS_COUNT * INFO_BLOCKS_COUNT, sizeof(int));
	return 0;
}

int next_step_duck(int* const area, int* const info_blocs, const SDL_Keycode* const key_code) {
	return -1;
}