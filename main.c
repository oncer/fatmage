#include "headers.h"
#include "defines.h"

#include <allegro.h>

BITMAP* buffer;
void swap( void )
{
	stretch_blit(buffer, screen, 0,0,WIDTH,HEIGHT, 0,0,800,480);
}

int frames;
void framec( void )
{
	frames++;
}

struct {
	int x,y; // current map offset
	unsigned char end;
	unsigned char state;
} game;

struct {
	BITMAP *tiles, *mage;
} Resource;

#include "map.h"

struct {
	int x, y;
	unsigned short walkc; // walk delay counter
	unsigned char dir; // direction
	unsigned char anim; // animation
	unsigned short animc; // animation delay counter
} mage;

void error(const char* str)
{
        fprintf(stderr, "%s\n", str);
}

int load_resources( void )
{
	Resource.tiles = load_pcx("data/tiles.pcx", 0);
	Resource.mage = load_pcx("data/mage.pcx", 0);
	if (Resource.tiles == NULL) {
		error("Couldn't load tiles.pcx");
		return -1;
	}
	if (Resource.mage == NULL) {
		error("Couldn't load mage.pcx");
		return -2;
	}
	return 0;
}

int destroy_resources( void )
{
	if (Resource.tiles != NULL) {
		destroy_bitmap(Resource.tiles);
	}
	if (Resource.mage != NULL) {
		destroy_bitmap(Resource.mage);
	}
	return 0;
}

int controls_normal()
{
	const int WD = 3;
	int anim = 0;
	if (key[KEY_DOWN]) {
		mage.walkc++;
		if (mage.walkc > WD) {
			mage.y++;
			mage.walkc = 0;
		}
		if (mage.y >= map.h*TILE-TILE) {
			mage.y = map.h*TILE-TILE;
		} else {
			mage.dir = D_DOWN;
			anim = 1;
		}
	} else if (key[KEY_UP]) {
		mage.walkc++;
		if (mage.walkc > WD) {
			mage.y--;
			mage.walkc = 0;
		}
		if (mage.y <= 0) {
			mage.y = 0;
		} else {
			mage.dir = D_UP;
			anim = 1;
		}
	} else if (key[KEY_RIGHT]) {
		mage.walkc++;
		if (mage.walkc > WD) {
			mage.x++;
			mage.walkc = 0;
		}
		if (mage.x >= map.w*TILE-TILE) {
			mage.x = map.w*TILE-TILE;
		} else {
			mage.dir = D_RIGHT;
			anim = 1;
		}
	} else if (key[KEY_LEFT]) {
		mage.walkc++;
		if (mage.walkc > WD) {
			mage.x--;
			mage.walkc = 0;
		}
		if (mage.x <= 0) {
			mage.x = 0;
		} else {
			mage.dir = D_LEFT;
			anim = 1;
		}
	}
	if (!anim) {
		mage.animc = 0;
		mage.anim = 0;
	} else if (mage.anim == 0) {
		mage.anim = 1;
	}

	return 0;
}

int controls_talking()
{
	return 0;
}

int controls_inventory()
{
	return 0;
}

int controls()
{
	if (key[KEY_ESC]) {
		game.end = 1;
	}
	
	switch (game.state) {
		case G_STATE_NORMAL:
			return controls_normal();
		case G_STATE_TALKING:
			return controls_talking();
		case G_STATE_INVENTORY:
			return controls_inventory();
		default:
			assert(0);
	}

	return 0;
}

int logic()
{
	if (mage.anim > 0)
	{
		mage.animc++;
		if (mage.animc > 7)
		{
			mage.animc = 0;
			mage.anim++;
			if (mage.anim >= MAGE_ANIMLEN) {
				mage.anim = 1;
			}
		}
	}
	if ((mage.x - game.x) > WIDTH*2/3) {
		game.x = mage.x - WIDTH*2/3;
	} else if ((mage.x - game.x) < WIDTH/3) {
		game.x = mage.x - WIDTH/3;
	}
	if ((mage.y - game.y) > HEIGHT*2/3) {
		game.y = mage.y - HEIGHT*2/3;
	} else if ((mage.y - game.y) < HEIGHT/3) {
		game.y = mage.y - HEIGHT/3;
	}
	if (game.x < 0) game.x = 0;
	if (game.x > map.w*TILE - WIDTH) game.x = map.w*TILE - WIDTH;
	if (game.y < 0) game.y = 0;
	if (game.y > map.h*TILE - HEIGHT) game.y = map.w*TILE - HEIGHT;

	return 0;
}

int display_objects()
{
	return 0;
}

int display_map()
{
	int offset_tx, offset_ty;
	int begin_x, begin_y;
	int end_tx, end_ty;
	offset_tx = game.x/TILE;
	offset_ty = game.y/TILE;
	begin_x = -game.x%TILE;
	begin_y = -game.y%TILE;
	end_tx = offset_tx + WIDTH/TILE + 1;
	end_ty = offset_ty + HEIGHT/TILE + 1;
	for (int y = offset_ty; y<end_ty; ++y)
	{
		for(int x = offset_tx; x<end_tx; ++x)
		{
			int t = map.tile[y*map.w+x];
			int t_y = t/16;
			int t_x = t%16;
			masked_blit(Resource.tiles, buffer, 
					t_x*TILE, t_y*TILE, 
					begin_x+(x-offset_tx)*TILE, begin_y+(y-offset_ty)*TILE, 
					TILE, TILE);
		}
	}
	return 0;
}

int display_mage()
{
	int offset_x, offset_y;
	offset_x = mage.x - game.x;
	offset_y = mage.y - game.y;
	masked_blit(Resource.mage, buffer, 
			mage.anim*TILE, mage.dir*TILE, 
			offset_x, offset_y, 
			TILE, TILE);
	return 0;
}

int game_loop()
{
	while (!game.end) {
		while (frames > 0)
		{
			controls();
			logic();
			frames--;
		}
		display_map();
		display_objects();
		display_mage();
		swap();
		while (frames <= 0) {
			rest(1);
		}
	}
	return 0;
}

int game_start()
{
	init_map();
	load_map("data/map01");
	game.x = 0;
	game.y = 0;
	game.end = 0;
	game.state = G_STATE_NORMAL;

	mage.x = WIDTH/2;
	mage.y = HEIGHT/2;
	mage.dir = D_RIGHT;
	mage.walkc = 0;
	mage.anim = 0;
	mage.animc = 0;
	game_loop();

	return 0;
}

int menu()
{
	game_start();
	return 0;
}

int init()
{
	allegro_init();
	install_keyboard();
	install_timer();
	install_sound(DIGI_AUTODETECT, MIDI_NONE, 0);
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 480, 0, 0);
	buffer = create_bitmap(WIDTH,HEIGHT);
	clear(buffer);
	load_resources();

	install_int_ex(framec, BPS_TO_TIMER(FRAMES_PER_SECOND));
	return 0;
}

int quit()
{
	remove_int(framec);

	destroy_resources();
	destroy_bitmap(buffer);
	set_gfx_mode(GFX_TEXT, 640, 480, 0, 0);
	allegro_exit();
	return 0;
}

int main(int argc, char** argv)
{
	init();
	menu();
	quit();
	return 0;
}
END_OF_MAIN()

