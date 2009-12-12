#ifndef __MAP_H
#define __MAP_H

#include "defines.h"

typedef struct {
	unsigned char type;
	unsigned short x,y;
	unsigned int state; //flags
	char* script_name;
} OBJECT;

struct {
	unsigned short w, h;
	unsigned char* tile;
	OBJECT object[FMMF_OBJECTS];
} map;

extern void init_map( void );
extern void new_map( int w, int h );
extern int load_map( const char* filename );
extern int save_map( const char* filename );

#endif

