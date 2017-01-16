#pragma once
#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
typedef enum {
	INIT_MENU_OK = 0,
	INIT_MENU_ERROR
} menu_init_code;
typedef struct menuelement {
	int menu_image[200];
	int(*init)();
	int(*next_step)();
	/*return score or -1 if gameover*/
} menuelement;

typedef struct menu {
	int count;
	int capacity;
	int choice;
	menuelement *menuelement;
} menu;

//void add_menu_item(menu *menu, int* menu_image, int(*init)(), int(*next_step)());

int init_menu(menu* menu);
#endif