#include "SDL.h"
#include <stdio.h>
#include <time.h>
#include "menu.h"
#include "app_window.h"

void delay(size_t mils);
int main(int argc, char* args[]) {
	int* area = NULL;
	size_t row, colomn, i;
	menu mainmenu;
	if (init_gui()) {
		printf("SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	if ((area = (int*)malloc(sizeof(int) * 200)) == NULL) {
		printf("Playground array not created!\n");
		exit(2);
	}
	create_gui();

	init_menu(&mainmenu);

	choose_menu_item(&mainmenu);

	render(mainmenu.menuelement[mainmenu.choice].menu_image);

	delay(1000);

	for (i = 0; i < 50; ++i) {
		printf("%lu\n", i);
		for (row = 0; row < PLAYGROUND_ROWS; ++row) {
			for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
				area[row * PLAYGROUND_COLOMNS + colomn] = (row + colomn + i) % 3;
			}
		}
		render(area);
		delay(100);
	}

	delay(100);
	//SDL_Delay(5000);
	
	fill_red_test();

	delay(2000);
	destroy_gui();
	free(area);
	return 0;
}

void delay(size_t mils) {
	long start = clock();
	while (clock() - start < ((double)mils) * CLOCKS_PER_SEC / 1000);
}