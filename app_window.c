#define _CRT_SECURE_NO_WARNINGS
#include "app_window.h"
#include "game.h"
#include "menu.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include <stdio.h>

/*Coordinates and sizes*/
#define BRICK_SIZE 24
#define BORDER 3
#define BRICK_SIZE_WITH_BORDER (BRICK_SIZE+BORDER)
#define FONT_SIZE (BRICK_SIZE-BORDER)
#define INFO_AREA_X (5 * BORDER + BRICK_SIZE_WITH_BORDER * PLAYGROUND_COLOMNS)
#define INFO_AREA_Y (5 * BORDER + BRICK_SIZE_WITH_BORDER * PLAYGROUND_ROWS)

/*Colors*/
#define BACKGROUND_COLOR 0x6D775C
#define BLACK_COLOR 0x000000
#define EMPTY_BLOCKS_COLOR 0x61705B

static const SDL_Color TEXT_COLOR = { 0,0,0,0 };/*black color*/
static const SDL_Color EMPTY_TEXT_COLOR = { 0x61,0x70,0x5B,0 };/*color same empty block*/

/*Paths*/
/*#define FONT_PATH "C:/Windows/Fonts/ARIALNB.ttf"*/
#define FONT_PATH "../res/ARIALNB.TTF"

static const int SCREEN_WIDTH = INFO_AREA_X + INFO_BLOCKS_COUNT * BRICK_SIZE_WITH_BORDER;
static const int SCREEN_HEIGHT = INFO_AREA_Y + INFO_BLOCKS_COUNT * BRICK_SIZE_WITH_BORDER;

static SDL_Window* window = NULL;
static SDL_Surface* screenSurface = NULL;
/*Text block's coordinates*/
static const SDL_Rect hi_score_num_dest = {
	INFO_AREA_X + BORDER /*+ BRICK_SIZE_WITH_BORDER*/,
	3 * BORDER + BRICK_SIZE_WITH_BORDER,
	BRICK_SIZE_WITH_BORDER * 4,
	BRICK_SIZE_WITH_BORDER
};
static const SDL_Rect score_num_dest = {
	INFO_AREA_X + BORDER /*+ BRICK_SIZE_WITH_BORDER*/,
	3 * BORDER + 3 * BRICK_SIZE_WITH_BORDER,
	BRICK_SIZE_WITH_BORDER * 4,
	BRICK_SIZE_WITH_BORDER
};

static const SDL_Rect speed_num_dest = {
	INFO_AREA_X + BORDER /*+ BRICK_SIZE_WITH_BORDER*/,
	3 * BORDER + 12 * BRICK_SIZE_WITH_BORDER,
	BRICK_SIZE_WITH_BORDER * 4,
	BRICK_SIZE_WITH_BORDER
};

static const SDL_Rect pause_text_dest = {
	INFO_AREA_X + BORDER /*+ BRICK_SIZE_WITH_BORDER*/,
	3 * BORDER + 15 * BRICK_SIZE_WITH_BORDER,
	BRICK_SIZE_WITH_BORDER * 4,
	BRICK_SIZE_WITH_BORDER
};

static const SDL_Rect game_text_dest = {
	INFO_AREA_X + BORDER /*+ BRICK_SIZE_WITH_BORDER*/,
	3 * BORDER + 16 * BRICK_SIZE_WITH_BORDER,
	BRICK_SIZE_WITH_BORDER * 4,
	BRICK_SIZE_WITH_BORDER
};

static const SDL_Rect over_text_dest = {
	INFO_AREA_X + BORDER /*+ BRICK_SIZE_WITH_BORDER*/,
	3 * BORDER + 17 * BRICK_SIZE_WITH_BORDER,
	BRICK_SIZE_WITH_BORDER * 4,
	BRICK_SIZE_WITH_BORDER
};

static const SDL_Rect start_text_dest = {
	3 * BORDER + 5 * BRICK_SIZE_WITH_BORDER,
	INFO_AREA_Y + BRICK_SIZE_WITH_BORDER,
	BRICK_SIZE_WITH_BORDER * 4,
	BRICK_SIZE_WITH_BORDER
};
static const SDL_Rect space_text_dest = {
	6 * BORDER + 10 * BRICK_SIZE_WITH_BORDER,
	INFO_AREA_Y,
	BRICK_SIZE_WITH_BORDER * 4,
	BRICK_SIZE_WITH_BORDER
};
static const SDL_Rect space_func_text_dest = {
	6 * BORDER + 9 * BRICK_SIZE_WITH_BORDER,
	INFO_AREA_Y + BRICK_SIZE_WITH_BORDER,
	BRICK_SIZE_WITH_BORDER * 5,
	BRICK_SIZE_WITH_BORDER
};
static const SDL_Rect up_down_text_dest = {
	3 * BORDER + 2 * BRICK_SIZE_WITH_BORDER,
	INFO_AREA_Y + 3 * BRICK_SIZE_WITH_BORDER,
	BRICK_SIZE_WITH_BORDER * 4,
	BRICK_SIZE_WITH_BORDER
};
static const SDL_Rect speed_up_down_text_dest = {
	3 * BORDER + BRICK_SIZE_WITH_BORDER,
	INFO_AREA_Y + 3 * BRICK_SIZE_WITH_BORDER,
	BRICK_SIZE_WITH_BORDER * 6,
	BRICK_SIZE_WITH_BORDER
};

static const SDL_Rect left_right_text_dest = {
	3 * BORDER + 8 * BRICK_SIZE_WITH_BORDER,
	INFO_AREA_Y + 3 * BRICK_SIZE_WITH_BORDER,
	BRICK_SIZE_WITH_BORDER * 5,
	BRICK_SIZE_WITH_BORDER
};

static void print_ttf(SDL_Surface *sDest, char* message, char* font, int size, SDL_Color color, SDL_Rect dest) {
	TTF_Font *fnt = TTF_OpenFont(font, size);
	SDL_Surface *sText = TTF_RenderText_Blended(fnt, message, color);
	SDL_BlitSurface(sText, NULL, sDest, &dest);
	SDL_FreeSurface(sText);
	TTF_CloseFont(fnt);
}

static void fill_playgraund(const long* const area) {
	size_t row, colomn;
	SDL_Rect rect, rect1, rect2, rect3;
	rect3.h = PLAYGROUND_ROWS * BRICK_SIZE_WITH_BORDER;
	rect3.w = PLAYGROUND_COLOMNS * BRICK_SIZE_WITH_BORDER;
	rect3.x = BORDER * 3;
	rect3.y = BORDER * 3;
	SDL_FillRect(screenSurface, &rect3, BACKGROUND_COLOR);
	rect.h = BRICK_SIZE;
	rect.w = BRICK_SIZE;
	rect1.h = rect.h - 2 * BORDER;
	rect1.w = rect.w - 2 * BORDER;
	rect2.h = rect1.h - 2 * BORDER;
	rect2.w = rect1.h - 2 * BORDER;
	for (row = 0; row < PLAYGROUND_ROWS; ++row) {
		for (colomn = 0; colomn < PLAYGROUND_COLOMNS; ++colomn) {
			uint32_t block_color;
			if (0 != area[row * PLAYGROUND_COLOMNS + colomn]) {
				block_color = BLACK_COLOR;
			}
			else {
				block_color = EMPTY_BLOCKS_COLOR;
			}
			rect.x = rect3.x + (int)colomn * BRICK_SIZE_WITH_BORDER;
			rect.y = rect3.y + (int)row * BRICK_SIZE_WITH_BORDER;
			SDL_FillRect(screenSurface, &rect, block_color);
			rect1.x = rect.x + BORDER;
			rect1.y = rect.y + BORDER;
			SDL_FillRect(screenSurface, &rect1, BACKGROUND_COLOR);
			rect2.x = rect1.x + BORDER;
			rect2.y = rect1.y + BORDER;
			SDL_FillRect(screenSurface, &rect2, block_color);
		}
	}
}

static void fill_info_blocks(const long* const info_blocks) {
	size_t row, colomn;
	SDL_Rect rect1, rect2, rect3;
	SDL_Rect rect = {
		INFO_AREA_X,
		3 * BORDER + 5 * BRICK_SIZE_WITH_BORDER,
		BRICK_SIZE_WITH_BORDER * 4,
		BRICK_SIZE_WITH_BORDER * 4
	};
	rect1.h = BRICK_SIZE;
	rect1.w = BRICK_SIZE;
	rect2.h = rect1.h - 2 * BORDER;
	rect2.w = rect1.w - 2 * BORDER;
	rect3.h = rect2.h - 2 * BORDER;
	rect3.w = rect2.h - 2 * BORDER;
	SDL_FillRect(screenSurface, &rect, BACKGROUND_COLOR);
	for (row = 0; row < INFO_BLOCKS_COUNT; ++row) {
		for (colomn = 0; colomn < INFO_BLOCKS_COUNT; ++colomn) {
			uint32_t block_color;
			if (0 != info_blocks[row * INFO_BLOCKS_COUNT + colomn]) {
				block_color = BLACK_COLOR;
			}
			else {
				block_color = EMPTY_BLOCKS_COLOR;
			}
			rect1.x = rect.x + (int)colomn * BRICK_SIZE_WITH_BORDER;
			rect1.y = rect.y + (int)row * BRICK_SIZE_WITH_BORDER;
			SDL_FillRect(screenSurface, &rect1, block_color);
			rect2.x = rect1.x + BORDER;
			rect2.y = rect1.y + BORDER;
			SDL_FillRect(screenSurface, &rect2, BACKGROUND_COLOR);
			rect3.x = rect2.x + BORDER;
			rect3.y = rect2.y + BORDER;
			SDL_FillRect(screenSurface, &rect3, block_color);
		}
	}
}
/*set functions*/
void set_hi_score_num(int hi_score) {
	char* const hi_score_text = (char*)malloc(7 * sizeof(char));
	sprintf(hi_score_text, "%d", hi_score);
	SDL_FillRect(screenSurface, &hi_score_num_dest, BACKGROUND_COLOR);
	print_ttf(screenSurface, hi_score_text, FONT_PATH, FONT_SIZE, TEXT_COLOR, hi_score_num_dest);
	free(hi_score_text);
	SDL_UpdateWindowSurface(window);
}
void set_score_num(int score) {
	char* const score_text = (char*)malloc(7 * sizeof(char));
	sprintf(score_text, "%d", score);
	SDL_FillRect(screenSurface, &score_num_dest, BACKGROUND_COLOR);
	print_ttf(screenSurface, score_text, FONT_PATH, FONT_SIZE, TEXT_COLOR, score_num_dest);
	free(score_text);
	SDL_UpdateWindowSurface(window);
}

void set_speed_num(int speed) {
	char* const speed_text = (char*)malloc(7 * sizeof(char));
	sprintf(speed_text, "%d", speed);
	SDL_FillRect(screenSurface, &speed_num_dest, BACKGROUND_COLOR);
	print_ttf(screenSurface, speed_text, FONT_PATH, FONT_SIZE, TEXT_COLOR, speed_num_dest);
	free(speed_text);
	SDL_UpdateWindowSurface(window);
}

void set_pause_text(enum text_show_t text_show) {
	SDL_Color color;
	if (text_show) {
		color = TEXT_COLOR;
	}
	else {
		color = EMPTY_TEXT_COLOR;
	}
	SDL_FillRect(screenSurface, &pause_text_dest, BACKGROUND_COLOR);
	print_ttf(screenSurface, "Pause", FONT_PATH, FONT_SIZE, color, pause_text_dest);
	SDL_UpdateWindowSurface(window);
}
void set_game_over_text(enum text_show_t text_show) {
	SDL_Color color;
	if (text_show) {
		color = TEXT_COLOR;
	}
	else {
		color = EMPTY_TEXT_COLOR;
	}
	SDL_FillRect(screenSurface, &game_text_dest, BACKGROUND_COLOR);
	SDL_FillRect(screenSurface, &over_text_dest, BACKGROUND_COLOR);
	print_ttf(screenSurface, "GAME", FONT_PATH, FONT_SIZE, color, game_text_dest);
	print_ttf(screenSurface, "OVER", FONT_PATH, FONT_SIZE, color, over_text_dest);
	SDL_UpdateWindowSurface(window);
}

void set_info_text(enum info_text_state_t info_text_state) {
	SDL_FillRect(screenSurface, &start_text_dest, BACKGROUND_COLOR);
	SDL_FillRect(screenSurface, &space_text_dest, BACKGROUND_COLOR);
	SDL_FillRect(screenSurface, &space_func_text_dest, BACKGROUND_COLOR);
	SDL_FillRect(screenSurface, &speed_up_down_text_dest, BACKGROUND_COLOR);
	SDL_FillRect(screenSurface, &left_right_text_dest, BACKGROUND_COLOR);
	if (info_text_state) {
		print_ttf(screenSurface, "Start", FONT_PATH, FONT_SIZE, TEXT_COLOR, start_text_dest);
		print_ttf(screenSurface, "Speed up / down", FONT_PATH, FONT_SIZE, TEXT_COLOR, speed_up_down_text_dest);
		print_ttf(screenSurface, "Choose game", FONT_PATH, FONT_SIZE, TEXT_COLOR, left_right_text_dest);
	}
	else {
		print_ttf(screenSurface, "Pause", FONT_PATH, FONT_SIZE, TEXT_COLOR, start_text_dest);
		print_ttf(screenSurface, "SPACE", FONT_PATH, FONT_SIZE, TEXT_COLOR, space_text_dest);
		print_ttf(screenSurface, "Boost / Rotate", FONT_PATH, FONT_SIZE, TEXT_COLOR, space_func_text_dest);
		print_ttf(screenSurface, "Navigation", FONT_PATH, FONT_SIZE, TEXT_COLOR, up_down_text_dest);
		print_ttf(screenSurface, "Navigation", FONT_PATH, FONT_SIZE, TEXT_COLOR, left_right_text_dest);
	}
	SDL_UpdateWindowSurface(window);
}

static void create_text(void) {
	SDL_Rect dest;
	/*Hi-Score label*/
	dest.x = INFO_AREA_X /*+ BRICK_SIZE_WITH_BORDER*/;
	dest.y = 3 * BORDER;
	dest.w = BRICK_SIZE_WITH_BORDER * 4;
	dest.h = BRICK_SIZE_WITH_BORDER;
	print_ttf(screenSurface, "Hi-Score", FONT_PATH, FONT_SIZE, TEXT_COLOR, dest);
	/*Hi-Score number*/
	set_hi_score_num(0);
	/*Score label*/
	dest.x = INFO_AREA_X /*+ BRICK_SIZE_WITH_BORDER*/;
	dest.y = 3 * BORDER + 2 * BRICK_SIZE_WITH_BORDER;
	print_ttf(screenSurface, "Score", FONT_PATH, FONT_SIZE, TEXT_COLOR, dest);
	/*Score number*/
	set_score_num(0);
	/*Speed label*/
	dest.x = INFO_AREA_X /*+ BRICK_SIZE_WITH_BORDER*/;
	dest.y = 3 * BORDER + 11 * BRICK_SIZE_WITH_BORDER;
	print_ttf(screenSurface, "Speed", FONT_PATH, FONT_SIZE, TEXT_COLOR, dest);
	/*Speed number*/
	set_speed_num(0);
	/*Pause*/
	set_pause_text(TEXT_SHOW_OFF);
	/*Game over*/
	set_game_over_text(TEXT_SHOW_OFF);
	/*Navigation info*/
	/*Escape key*/
	dest.x = 3 * BORDER;
	dest.y = INFO_AREA_Y;
	print_ttf(screenSurface, "ESC", FONT_PATH, FONT_SIZE, TEXT_COLOR, dest);
	dest.x = 3 * BORDER;
	dest.y = INFO_AREA_Y + BRICK_SIZE_WITH_BORDER;
	print_ttf(screenSurface, "Menu", FONT_PATH, FONT_SIZE, TEXT_COLOR, dest);
	dest.x = 3 * BORDER + 5 * BRICK_SIZE_WITH_BORDER;
	dest.y = INFO_AREA_Y;
	/*Enter key*/
	print_ttf(screenSurface, "ENTER", FONT_PATH, FONT_SIZE, TEXT_COLOR, dest);
	dest.x = 3 * BORDER + 2 * BRICK_SIZE_WITH_BORDER;
	dest.y = INFO_AREA_Y + 2 * BRICK_SIZE_WITH_BORDER;
	/*Navigation keys*/
	print_ttf(screenSurface, "UP / DOWN", FONT_PATH, FONT_SIZE, TEXT_COLOR, dest);
	dest.x = 3 * BORDER + 8 * BRICK_SIZE_WITH_BORDER;
	dest.y = INFO_AREA_Y + 2 * BRICK_SIZE_WITH_BORDER;
	print_ttf(screenSurface, "LEFT / RIGHT", FONT_PATH, FONT_SIZE, TEXT_COLOR, dest);
	/*Show menu keys info*/
	set_info_text(MENU_TEXT);
}

int is_event_correct(void* userdata, SDL_Event* event) {
	if (event != NULL) {
		if (event->type == SDL_KEYDOWN) {
			switch (event->key.keysym.sym) {
			case SDLK_DOWN:
				return 1;
			case SDLK_UP:
				return 1;
			case SDLK_RIGHT:
				return 1;
			case SDLK_LEFT:
				return 1;
			case SDLK_RETURN:
				return 1;
			case SDLK_ESCAPE:
				return 1;
			case SDLK_SPACE:
				return 1;
			default:
				return 0;
			}
		}
		if (event->type == SDL_QUIT) return 1;
		if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESTORED) return 1;
	}
	return 0;
}

int init_gui(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return SDL_INIT_ERROR;
	window = SDL_CreateWindow("Brick Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) return SDL_CREATE_WINDOW_ERROR;
	screenSurface = SDL_GetWindowSurface(window);
	if (screenSurface == NULL) return SDL_GET_SURFACE_ERROR;
	SDL_SetEventFilter(is_event_correct, NULL);
	return SDL_INIT_EVERYTHING_OK;
}

int create_gui(void) {
	SDL_Rect rect;
	long info_blocs[16] = {	0,0,0,0,
							0,0,0,0,
							0,0,0,0,
							0,0,0,0 };
	SDL_FillRect(screenSurface, NULL, BACKGROUND_COLOR);
	rect.x = BORDER;
	rect.y = BORDER;
	rect.h = 3 * BORDER + BRICK_SIZE_WITH_BORDER * PLAYGROUND_ROWS;
	rect.w = 3 * BORDER + BRICK_SIZE_WITH_BORDER * PLAYGROUND_COLOMNS;
	SDL_FillRect(screenSurface, &rect, BLACK_COLOR);
	rect.x = 2 * BORDER;
	rect.y = 2 * BORDER;
	rect.h = BORDER + BRICK_SIZE_WITH_BORDER * PLAYGROUND_ROWS;
	rect.w = BORDER + BRICK_SIZE_WITH_BORDER * PLAYGROUND_COLOMNS;
	SDL_FillRect(screenSurface, &rect, BACKGROUND_COLOR);
	create_text();
	fill_info_blocks(info_blocs);
	SDL_UpdateWindowSurface(window);
	return 0;
}

void render(const long* const area, const long* const info_blocs) {
	fill_playgraund(area);
	if (info_blocs) fill_info_blocks(info_blocs);
	SDL_UpdateWindowSurface(window);
}

void destroy_gui(void) {
	SDL_FreeSurface(screenSurface);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit();
	free_menu();
	free_areas();
}