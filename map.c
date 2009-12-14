#include "common.h"
#include "map.h"
#include "game.h"

void init_map()
{
	map.w = 0;
	map.h = 0;
	map.tile = NULL;
	for (int i=0; i<FMMF_OBJECTS; ++i)
	{
		map.object[i].type = O_NONE;
	}
}

static void clear_map()
{
	if (map.tile != NULL)
	{
		free(map.tile);
		map.w = 0;
		map.h = 0;
	}
}

void new_map(int w, int h)
{
	clear_map();
	map.w = w;
	map.h = h;
	map.tile = (unsigned char*)malloc(w*h);
	memset(map.tile,0,w*h);
	for (int i=0; i<FMMF_OBJECTS; ++i)
	{
		map.object[i].type = O_NONE;
	}
}


/**
 * Error codes:
 *   0: No error
 *  -1: Unknown error
 *  -2: Wrong map format
 *  -3: Map file version too recent
 *  -4: Could not open file
 */
int load_map( const char* filename )
{
	char* ff[4];
	uint16_t version;
	int f = open(filename, O_RDONLY);
	if (f == -1) {
		return -4;
	}
	read(f,ff,4);
	if (strncmp((const char*)ff,"FMMF",4) != 0) {
		return -2;
	}
	read(f,&version,2);
	if (version > FMMF_VERSION) {
		return -3;
	}
	clear_map();
	read(f,&map.w,sizeof(unsigned short));
	read(f,&map.h,sizeof(unsigned short));
	map.tile = (unsigned char*)malloc(map.w*map.h);
	read(f,map.tile,map.w*map.h);
	for (int i=0; i<FMMF_OBJECTS; ++i) {
		read(f,&map.object[i].type,1);
		read(f,&map.object[i].x,sizeof(unsigned short));
		read(f,&map.object[i].y,sizeof(unsigned short));
		read(f,&map.object[i].state,sizeof(unsigned int));
	}
	close(f);
	return 0;
}

/**
 *  0: No error
 * -1: Unknown error
 * -4: Couldn't open file
 */
int save_map( const char* filename )
{
	char ff[4] = "FMMF";
	uint16_t version = FMMF_VERSION;
	int f = open(filename, O_WRONLY | O_CREAT, 00644 );
	if (f == -1) {
		return -4;
	}
	write(f,ff,4);
	write(f,&version,2);
	write(f,&map.w,sizeof(unsigned short));
	write(f,&map.h,sizeof(unsigned short));
	write(f,map.tile,map.w*map.h);
	for (int i=0; i<FMMF_OBJECTS; ++i) {
		write(f,&map.object[i].type,1);
		write(f,&map.object[i].x,sizeof(unsigned short));
		write(f,&map.object[i].y,sizeof(unsigned short));
		write(f,&map.object[i].state,sizeof(unsigned int));
	}
	close(f);
	return 0;
}

