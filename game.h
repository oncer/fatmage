#ifndef __GAME_H
#define __GAME_H

#include "defines.h"

struct {
	int x,y; // current map offset
	unsigned char end;
	unsigned char state;
} game;

extern int game_start( void );

#endif

