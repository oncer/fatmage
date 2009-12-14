#include "game.h"
#include "common.h"
#include "mage.h"
#include "map.h"
#include "rules.h"

static int controls_normal()
{
	const int WD = 3;
	int anim = 0;
	if (key[KEY_DOWN]) {
		mage.walkc++;
		if (mage.walkc > WD) {
			mage.y++;
                        rules_fire(E_POSITION_CHANGE, NULL);
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
                        rules_fire(E_POSITION_CHANGE, NULL);
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
                        rules_fire(E_POSITION_CHANGE, NULL);
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
                        rules_fire(E_POSITION_CHANGE, NULL);
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

static int controls_talking()
{
	return 0;
}

static int controls_inventory()
{
	return 0;
}

static int controls()
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

static int logic()
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

static int display_mage()
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

static int display_map()
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

static int game_loop()
{
	while (!game.end) {
		while (frames > 0)
		{
			controls();
			logic();
			frames--;
		}
		display_map();
		//display_objects();
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

