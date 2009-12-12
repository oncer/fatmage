#include "headers.h"
#include "defines.h"
#include "map.h"

#include <allegro.h>

static BITMAP* buffer;
static void swap()
{
	blit(buffer,screen,0,0,0,0,1024,768);
}

static BITMAP* tiles;

struct {
	unsigned char sel; // selected tile
	int x,y; // map display offset
	int map; // open map number
} edit;

int open_map(int);
int write_map(int);

void input()
{
	int old_map = edit.map;
	if (key[KEY_1]) {
		edit.map = 1;
	} else if (key[KEY_2]) {
		edit.map = 2;
	} else if (key[KEY_3]) {
		edit.map = 3;
	} else if (key[KEY_4]) {
		edit.map = 4;
	} else if (key[KEY_5]) {
		edit.map = 5;
	} else if (key[KEY_6]) {
		edit.map = 6;
	} else if (key[KEY_7]) {
		edit.map = 7;
	} else if (key[KEY_8]) {
		edit.map = 8;
	} else if (key[KEY_9]) {
		edit.map = 9;
	} else if (key[KEY_0]) {
		edit.map = 10;
	}
	if (old_map != edit.map) {
		write_map(old_map);
		open_map(edit.map);
	}

	if (key[KEY_LEFT]) {
		if (edit.x > 0) { edit.x--; }
	} else if (key[KEY_RIGHT]) {
		if (edit.x < map.w-1-1024/TILE) { edit.x++; }
	}
	if (key[KEY_UP]) {
		if (edit.y > 0) { edit.y--; }
	} else if (key[KEY_DOWN]) {
		if (edit.y < map.h-1-768/TILE) { edit.y++; }
	}

	poll_mouse();
	if (mouse_b & 1) { //left mouse button
		int tX = mouse_x/TILE;
		int tY = mouse_y/TILE;
		if (mouse_y < TILE*4) {
			// select tile
			edit.sel = tY*64 + tX;
		} else {
			// draw tile
			int index = (edit.y + (tY-4)) * map.w + (edit.x + tX);
			map.tile[index] = edit.sel;
		}
	}
}

void draw_tiles()
{
	for (int i=0; i<256; ++i)
	{
		blit(tiles,buffer,(i%16)*TILE,(i/16)*TILE,(i%64)*TILE,(i/64)*TILE,TILE,TILE);
	}
	int tX = edit.sel%64;
	int tY = edit.sel/64;
	rect(buffer, tX*TILE, tY*TILE, tX*TILE + TILE, tY*TILE + TILE, makecol32(255,255,0));
}

void draw_map()
{
	for (int y = edit.y; y < edit.y + 768/TILE - 4; ++y) {
		for (int x = edit.x; x < edit.x + 1024/TILE; ++x) {
			unsigned char t = map.tile[y*map.w+x];
			blit(tiles,buffer,(t%16)*TILE,(t/16)*TILE,(x-edit.x)*TILE,(y-edit.y+4)*TILE,TILE,TILE);
		}
	}
}

void draw_cursor()
{
	int tX = mouse_x - mouse_x%TILE;
	int tY = mouse_y - mouse_y%TILE;
	rect(buffer, tX, tY, tX+TILE, tY+TILE, makecol32(200,200,255));
	rect(buffer, mouse_x-1, mouse_y-1, mouse_x+1, mouse_y+1, makecol32(200,200,255));
}

int open_map(int n)
{
	char name[11];
	int file_exists;
	if (n > 99 || n < 0) return -1;
	snprintf(name, 11, "data/map%02d", n);
	file_exists = access(name, R_OK);
	if (file_exists != 0)
	{
		new_map(64,64);
		save_map(name);
	}
	return load_map(name);
}

int write_map(int n)
{
	char name[11];
	if (n > 99 || n < 0) return -1;
	snprintf(name, 11, "data/map%02d", n);
	return save_map(name);
}

int main(int argc, char** argv)
{
	allegro_init();
	install_keyboard();
	install_mouse();
	install_timer();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,1024,768,0,0);
	buffer = create_bitmap(1024,768);
	tiles = load_pcx("data/tiles.pcx",0);
	clear(buffer);

	edit.sel = 0;
	edit.x = 0;
	edit.y = 0;

	init_map();
	open_map(1);

	while (!key[KEY_ESC])
	{
		input();
		draw_tiles();
		draw_map();
		draw_cursor();
		swap();
		rest(10);
	}
	write_map(edit.map);

	destroy_bitmap(tiles);
	destroy_bitmap(buffer);
	allegro_exit();
	return 0;
}

