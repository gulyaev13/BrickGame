#include "app_window.h"
#include "SDL.h"

static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;
static SDL_Window* window = NULL;
static SDL_Surface* screenSurface = NULL;

int init_gui() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return SDL_INIT_ERROR;
	window = SDL_CreateWindow("Brick Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) return SDL_CREATE_WINDOW_ERROR;
	screenSurface = SDL_GetWindowSurface(window);
	if (screenSurface == NULL) return SDL_GET_SURFACE_ERROR;
	return SDL_INIT_EVERYTHING_OK;
}

int create_gui() {
	SDL_Rect rect;
	SDL_FillRect(screenSurface, NULL, BACKGROUND_COLOR);
	rect.x = BORDER_START_X;
	rect.y = BORDER_START_Y;
	rect.h = 4 * BORDER_START_Y + BRICK_SIZE_WITH_BORDER * PLAYGROUND_ROWS - (BRICK_SIZE_WITH_BORDER - BRICK_SIZE);
	rect.w = 4 * BORDER_START_X + BRICK_SIZE_WITH_BORDER * PLAYGROUND_COLOMNS - (BRICK_SIZE_WITH_BORDER - BRICK_SIZE);
	SDL_FillRect(screenSurface, &rect, BLACK_COLOR);
	rect.x = 2 * BORDER_START_X;
	rect.y = 2 * BORDER_START_Y;
	rect.h = 2 * BORDER_START_Y + BRICK_SIZE_WITH_BORDER * PLAYGROUND_ROWS - (BRICK_SIZE_WITH_BORDER - BRICK_SIZE);
	rect.w = 2 * BORDER_START_X + BRICK_SIZE_WITH_BORDER * PLAYGROUND_COLOMNS - (BRICK_SIZE_WITH_BORDER - BRICK_SIZE);
	SDL_FillRect(screenSurface, &rect, BACKGROUND_COLOR);
	//Добавить отображение счёта
	SDL_UpdateWindowSurface(window);
	return 0;
}

static void fill_playgraund(const int* const area) {
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

void fill_red_test() {
	SDL_FillRect(screenSurface, NULL, RED_COLOR);
	SDL_UpdateWindowSurface(window);
}

void render(const int* const area) {
	fill_playgraund(area);
	SDL_UpdateWindowSurface(window);
}

void destroy_gui() {
	SDL_FreeSurface(screenSurface);
	SDL_DestroyWindow(window);
	SDL_Quit();
}