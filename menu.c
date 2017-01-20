#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*Games*/
#include "duck.h"
#include "snake.h"
#include "galaxy.h"

#include "SDL2/SDL.h"
#include "menu.h"
#include "app_window.h"

typedef struct menuelement_t {
	const int* menu_image;
	int hi_score;
	int(*init)(int**, int**);
	int(*next_step)(int* const, int* const, const SDL_Keycode* const);
	/*return score or -1 if gameover*/
} menuelement_t;

typedef struct menu_t {
	int count;
	int capacity;
	int choice;
	int speed;
	menuelement_t *menuelement;
} menu_t;


#define MAX_SPEED 9
#define MIN_SPEED 0
#define MAX_HI_SCORE 999999

static menu_t menu;

static void add_menu_item(const int* menu_image, int hi_score, int(*init)(int**, int**), int(*next_step)(int* const, int* const, const SDL_Keycode* const)) {
	int working = 1;
	while (working) {
		if (menu.count + 1 <= menu.capacity) {
			menu.menuelement[menu.count].init = init;
			menu.menuelement[menu.count].next_step = next_step;
			menu.menuelement[menu.count].menu_image = menu_image;
			menu.menuelement[menu.count].hi_score = hi_score;
			menu.count++;
			working = 0;
		}
		else {
			if ((menu.menuelement = (menuelement_t*)realloc(menu.menuelement, sizeof(menuelement_t) * (menu.capacity + 1))) == NULL) {
				printf("Failed memory location!\n");
				break;
			}
			menu.capacity++;
		}
	}
}

int init_menu() {
	menu.capacity = 2;
	menu.count = 0;
	menu.choice = 0;
	menu.speed = 0;
	if ((menu.menuelement = (menuelement_t*)malloc(sizeof(menuelement_t) * 2)) == NULL) return INIT_MENU_ERROR;
	add_menu_item(menu_image_snake, 0, init_snake, next_step_snake);
	add_menu_item(menu_image_galaxy, 0, init_galaxy, next_step_galaxy);
	add_menu_item(menu_image_duck, 0, init_duck, next_step_duck); 
	return INIT_MENU_OK;
}

void choose_menu_item() {
	SDL_Event event;
	int working = 1;
	render(menu.menuelement[menu.choice].menu_image, NULL);
	while (working) {
		SDL_WaitEvent(&event);
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_DOWN:
					menu.speed = (menu.speed - 1) < MIN_SPEED ? MAX_SPEED : (menu.speed - 1);
					set_speed_num(menu.speed);
					break;
				case SDLK_UP:
					menu.speed = (menu.speed + 1) > MAX_SPEED ? MIN_SPEED : (menu.speed + 1);
					set_speed_num(menu.speed);
					break;
				case SDLK_RIGHT:
					menu.choice = (menu.choice + 1) > (menu.count - 1) ? 0 : (menu.choice + 1);
					set_hi_score_num(menu.menuelement[menu.choice].hi_score);
					break;
				case SDLK_LEFT:
					menu.choice = (menu.choice - 1) < 0 ? (menu.count - 1) : (menu.choice - 1);
					set_hi_score_num(menu.menuelement[menu.choice].hi_score);
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
			}
			render(menu.menuelement[menu.choice].menu_image, NULL);
	}
}
void free_menu() {
	free(menu.menuelement);
}

int init_game(int** area, int** info_blocs) {
	return menu.menuelement[menu.choice].init(area, info_blocs);
}

int game_next_step(int* const area, int* const info_blocs, const SDL_Keycode* const key_code) {
	return menu.menuelement[menu.choice].next_step(area, info_blocs, key_code);
}

int get_speed() {
	return menu.speed;
}

void set_speed(int new_speed) {
	menu.speed = (new_speed > MAX_SPEED) ? MAX_SPEED : (new_speed < MIN_SPEED ? MIN_SPEED : new_speed);
}

int get_hi_score() {
	return menu.menuelement[menu.choice].hi_score;
}

void set_hi_score(int new_hi_score) {
	menu.menuelement[menu.choice].hi_score = new_hi_score > MAX_HI_SCORE ? MAX_HI_SCORE : new_hi_score;
}


