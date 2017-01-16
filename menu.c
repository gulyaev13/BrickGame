#include "menu.h"
#include "snake.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
static void add_menu_item(menu* menu, int* menu_image, int(*init)(), int(*next_step)()) {
	int working = 1;
	while (working) {
		if (menu->count + 1 <= menu->capacity) {
			menu->menuelement[menu->count].init = init;
			menu->menuelement[menu->count].next_step = next_step;
			memcpy(menu->menuelement[menu->count].menu_image, menu_image, 400 * sizeof(int));
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
	menu->capacity = 2;
	menu->count = 0;
	menu->choice = 0;
	menu->menuelement = (menuelement*)malloc(sizeof(menuelement) * 2);
	add_menu_item(menu, menu_image_snake, init_snake, next_step_snake);
	return INIT_MENU_OK;
}
