#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>
#include <time.h>
#include "menu.h"
#include "game.h"
#include "app_window.h"

int main(int argc, char* args[]) {
	if (init_gui()) {
		printf("SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	if (TTF_Init()) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	create_gui();
	if (init_menu()) {
		printf("Problem with menu!\n");
		exit(3);
	};
	do {
		choose_menu_item();
	} while (game_play());

	destroy_gui();
	return 0;
}

