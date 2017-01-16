#include "SDL.h"
#include <stdio.h>
#include <time.h>
#include "menu.h"

#define PLAYGROUND_COLOMNS 10
#define PLAYGROUND_ROWS 20

#define BRICK_SIZE 13
#define BRICK_SIZE_WITH_BORDER 14

#define BORDER_START_X 2
#define BORDER_START_Y 2

/*Colors*/
#define BACKGROUND_COLOR 0xD3D3D3
#define INDIGO_COLOR 0x4B0082
#define RED_COLOR 0xFF0000
#define BLACK_COLOR 0x000000
#define DARK_GREY_COLOR 0x808080

typedef enum {
	SDL_INIT_EVERYTHING_OK = 0,
	SDL_INIT_ERROR,
	SDL_CREATE_WINDOW_ERROR,
	SDL_GET_SURFACE_ERROR,
} gui_init_t;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
void fill_playgraund(SDL_Surface* screenSurface, const int* const area);
void delay(size_t mils);

int init_gui(SDL_Window** window, SDL_Surface** screenSurface) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return SDL_INIT_ERROR;
	*window = SDL_CreateWindow("Brick Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (*window == NULL) return SDL_CREATE_WINDOW_ERROR;
	*screenSurface = SDL_GetWindowSurface(*window);
	if (*screenSurface == NULL) return SDL_GET_SURFACE_ERROR;
	return SDL_INIT_EVERYTHING_OK;
}

int create_gui(SDL_Window** window, SDL_Surface** screenSurface) {
	SDL_Rect rect;
	SDL_FillRect(*screenSurface, NULL, BACKGROUND_COLOR);
	rect.x = BORDER_START_X;
	rect.y = BORDER_START_Y;
	rect.h = 4 * BORDER_START_Y + BRICK_SIZE_WITH_BORDER * PLAYGROUND_ROWS - (BRICK_SIZE_WITH_BORDER - BRICK_SIZE);
	rect.w = 4 * BORDER_START_X + BRICK_SIZE_WITH_BORDER * PLAYGROUND_COLOMNS - (BRICK_SIZE_WITH_BORDER - BRICK_SIZE);
	SDL_FillRect(*screenSurface, &rect, BLACK_COLOR);
	rect.x = 2 * BORDER_START_X;
	rect.y = 2 * BORDER_START_Y;
	rect.h = 2 * BORDER_START_Y + BRICK_SIZE_WITH_BORDER * PLAYGROUND_ROWS - (BRICK_SIZE_WITH_BORDER - BRICK_SIZE);
	rect.w = 2 * BORDER_START_X + BRICK_SIZE_WITH_BORDER * PLAYGROUND_COLOMNS - (BRICK_SIZE_WITH_BORDER - BRICK_SIZE);
	SDL_FillRect(*screenSurface, &rect, BACKGROUND_COLOR);
	//Добавить отображение счёта
	SDL_UpdateWindowSurface(*window);
	return 0;
}

void destroy(SDL_Window** window, SDL_Surface** screenSurface, int* area) {
	free(area);
	SDL_FreeSurface(*screenSurface);
	SDL_DestroyWindow(*window);
	SDL_Quit();
}
int main(int argc, char* args[]) {
	int* area = NULL;
	size_t row, colomn, i;
	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	menu mainmenu;
	if (init_gui(&window, &screenSurface)) {
		printf("SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	if ((area = (int*)malloc(sizeof(int) * 200)) == NULL) {
		printf("Playground array not created!\n");
		exit(2);
	}
	create_gui(&window, &screenSurface);

	init_menu(&mainmenu);

	fill_playgraund(screenSurface, mainmenu.menuelement[mainmenu.choice].menu_image);
	SDL_UpdateWindowSurface(window);
	
	SDL_Delay(5000);
	for (i = 0; i < 500; ++i) {
		printf("%lu\n", i);
		for (row = 0; row < PLAYGROUND_ROWS; ++row) {
			for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
				area[row * PLAYGROUND_COLOMNS + colomn] = (row + colomn + i) % 3;
			}
		}
		fill_playgraund(screenSurface, area);
		SDL_UpdateWindowSurface(window);

		delay(100);
	}

	SDL_FillRect(screenSurface, NULL, RED_COLOR);
	SDL_UpdateWindowSurface(window);
	delay(2000);

	destroy(&window, &screenSurface, area);

	return 0;
}
//Заполнение игрового поля из массива интов
void fill_playgraund(SDL_Surface* screenSurface, const int* const area) {
	int count = 0, row, colomn;
	SDL_Rect rect, rect1, rect2, rect3;
	rect3.h = PLAYGROUND_ROWS * BRICK_SIZE_WITH_BORDER;
	rect3.w = PLAYGROUND_COLOMNS * BRICK_SIZE_WITH_BORDER;
	rect3.x = BORDER_START_X * 3;
	rect3.y = BORDER_START_Y * 3;
	SDL_FillRect(screenSurface, &rect3, BACKGROUND_COLOR);
	rect.h = BRICK_SIZE;
	rect.w = BRICK_SIZE;
	rect1.h = rect.h - 2;
	rect1.w = rect.w - 2;
	rect2.h = rect.h - 4;
	rect2.w = rect.h - 4;
	for (row = 0; row < PLAYGROUND_ROWS; ++row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			uint32_t block_color;
			if (0 != area[row * PLAYGROUND_COLOMNS + colomn]) {
				block_color = BLACK_COLOR;
			}
			else {
				block_color = DARK_GREY_COLOR;
			}
			rect.x = rect3.x + colomn * BRICK_SIZE_WITH_BORDER;
			rect.y = rect3.y + row * BRICK_SIZE_WITH_BORDER;
			SDL_FillRect(screenSurface, &rect, block_color);
			rect1.x = rect.x + 1;
			rect1.y = rect.y + 1;
			SDL_FillRect(screenSurface, &rect1, BACKGROUND_COLOR);
			rect2.x = rect.x + 2;
			rect2.y = rect.y + 2;
			SDL_FillRect(screenSurface, &rect2, block_color);
		}
	}
}

void delay(size_t mils) {
	long start = clock();
	while (clock() - start < ((double)mils) * CLOCKS_PER_SEC / 1000);
}