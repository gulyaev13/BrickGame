#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*Games*/
#include "duck.h"
#include "snake.h"
#include "SDL.h"
#include "menu.h"
#include "app_window.h"


#define MAX_SPEED 9
#define MIN_SPEED 0

//static SDL_Event event;

static void add_menu_item(menu* menu, const int* menu_image, int hi_score, int(*init)(), int(*next_step)()) {
	int working = 1;
	while (working) {
		if (menu->count + 1 <= menu->capacity) {
			menu->menuelement[menu->count].init = init;
			menu->menuelement[menu->count].next_step = next_step;
			menu->menuelement[menu->count].menu_image = menu_image;
			menu->menuelement[menu->count].hi_score = hi_score;
			menu->count++;
			working = 0;
		}
		else {
			if ((menu->menuelement = (menuelement*)realloc(menu->menuelement, sizeof(menuelement) * (menu->capacity + 1))) == NULL) {
				printf("Failed memory location!\n");
				break;
			}
			menu->capacity++;
		}
	}
}

int init_menu(menu* menu) {
	printf("init_menu\n");
	menu->capacity = 2;
	menu->count = 0;
	menu->choice = 0;
	menu->speed = 0;
	menu->menuelement = (menuelement*)malloc(sizeof(menuelement) * 2);
	add_menu_item(menu, menu_image_snake, 0, init_snake, next_step_snake);
	add_menu_item(menu, menu_image_duck, 777, init_duck, next_step_duck);
	//printf("init_menu ok\n");
	return INIT_MENU_OK;
}

void choose_menu_item(menu* menu) {
	SDL_Event event;
	int working = 1;
	render(menu->menuelement[menu->choice].menu_image);
	while (working) {
		SDL_WaitEvent(&event);
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_DOWN:
					menu->speed = (menu->speed - 1) < MIN_SPEED ? MAX_SPEED : (menu->speed - 1);
					set_speed_num(menu->speed);
					//printf("Speed: %d\n", menu->speed);
					break;
				case SDLK_UP:
					menu->speed = (menu->speed + 1) > MAX_SPEED ? MIN_SPEED : (menu->speed + 1);
					set_speed_num(menu->speed);
					//printf("Speed: %d\n", menu->speed);
					break;
				case SDLK_RIGHT:
					menu->choice = (menu->choice + 1) > (menu->count - 1) ? 0 : (menu->choice + 1);
					set_hi_score_num(menu->menuelement[menu->choice].hi_score);
					break;
				case SDLK_LEFT:
					menu->choice = (menu->choice - 1) < 0 ? menu->count - 1 : (menu->choice - 1);
					set_hi_score_num(menu->menuelement[menu->choice].hi_score);
					break;
				case SDLK_RETURN:
					working = 0;
					break;
				}
				break;
			case SDL_QUIT:
				destroy_gui();
				exit(0);
				break;
			default:
				break;
			}
			render(menu->menuelement[menu->choice].menu_image);
	}
}



