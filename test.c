#include "tinyalleg.h"

int main(int argc, char** argv)
{
	talleg_init();
	ta_init();
	ta_color_depth(32);
	ta_gfx_mode(GFX_AUTODETECT_WINDOWED,64,64,0,0);
	ta_exit();
	return 0;
}
