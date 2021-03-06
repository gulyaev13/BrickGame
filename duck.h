#ifndef DUCK_H_INCLUDED
#define DUCK_H_INCLUDED
#include "SDL2/SDL.h"
const long menu_image_duck[200] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 1, 0, 1, 0, 1, 0, 1, 0,
	0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
	0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 1, 1, 1, 1, 1, 1, 0, 0
};

int init_duck(long** area, long** info_blocs);

int next_step_duck(long* const area, long* info_blocs, const SDL_Keycode* const key_code);
#endif