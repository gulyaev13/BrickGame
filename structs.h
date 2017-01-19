#pragma once
#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED
typedef struct {
	int x;
	int y;
} coordinates_t;

typedef enum {
	LEFT = -1,
	RIGHT = 1
} horizontal_move_t;
#endif
